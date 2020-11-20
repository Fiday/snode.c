/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020 Volker Christian <me@vchrist.at>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include "EventLoop.h"
#include "Logger.h"
#include "ResponseParser.h"
#include "ServerResponse.h"
#include "config.h" // just for this example app
#include "socket/bluetooth/rfcomm/Socket.h"
#include "socket/sock_stream/tls/SocketClient.h"

#include <openssl/x509v3.h>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

using namespace net::socket::stream;
using namespace net::socket::bluetooth;
using namespace net::socket::bluetooth::address;

tls::SocketClient<net::socket::bluetooth::rfcomm::Socket> getBtClient() {
    tls::SocketClient<rfcomm::Socket> btClient(
        []([[maybe_unused]] tls::SocketClient<rfcomm::Socket>::SocketConnection* socketConnection) -> void { // onConstruct
        },
        []([[maybe_unused]] tls::SocketClient<rfcomm::Socket>::SocketConnection* socketConnection) -> void { // onDestruct
        },
        [](tls::SocketClient<rfcomm::Socket>::SocketConnection* socketConnection) -> void { // onConnect
            VLOG(0) << "OnConnect";
            socketConnection->enqueue("Hello rfcomm connection!");

            VLOG(0) << "\tServer: " + socketConnection->getRemoteAddress().address() + "(" +
                           socketConnection->getRemoteAddress().address() +
                           "):" + std::to_string(socketConnection->getRemoteAddress().channel());
            VLOG(0) << "\tClient: " + socketConnection->getLocalAddress().address() + "(" + socketConnection->getLocalAddress().address() +
                           "):" + std::to_string(socketConnection->getLocalAddress().channel());
            X509* server_cert = SSL_get_peer_certificate(socketConnection->getSSL());
            if (server_cert != NULL) {
                int verifyErr = SSL_get_verify_result(socketConnection->getSSL());

                VLOG(0) << "     Server certificate: " + std::string(X509_verify_cert_error_string(verifyErr));

                char* str = X509_NAME_oneline(X509_get_subject_name(server_cert), 0, 0);
                VLOG(0) << "        Subject: " + std::string(str);
                OPENSSL_free(str);

                str = X509_NAME_oneline(X509_get_issuer_name(server_cert), 0, 0);
                VLOG(0) << "        Issuer: " + std::string(str);
                OPENSSL_free(str);

                // We could do all sorts of certificate verification stuff here before deallocating the certificate.

                GENERAL_NAMES* subjectAltNames =
                    static_cast<GENERAL_NAMES*>(X509_get_ext_d2i(server_cert, NID_subject_alt_name, NULL, NULL));

                int32_t altNameCount = sk_GENERAL_NAME_num(subjectAltNames);
                VLOG(0) << "        Subject alternative name count: " << altNameCount;
                for (int32_t i = 0; i < altNameCount; ++i) {
                    GENERAL_NAME* generalName = sk_GENERAL_NAME_value(subjectAltNames, i);
                    if (generalName->type == GEN_URI) {
                        std::string subjectAltName =
                            std::string(reinterpret_cast<const char*>(ASN1_STRING_get0_data(generalName->d.uniformResourceIdentifier)),
                                        ASN1_STRING_length(generalName->d.uniformResourceIdentifier));
                        VLOG(0) << "           SAN (URI): '" + subjectAltName;
                    } else if (generalName->type == GEN_DNS) {
                        std::string subjectAltName =
                            std::string(reinterpret_cast<const char*>(ASN1_STRING_get0_data(generalName->d.dNSName)),
                                        ASN1_STRING_length(generalName->d.dNSName));
                        VLOG(0) << "           SAN (DNS): '" + subjectAltName;
                    } else {
                        VLOG(0) << "           SAN (Type): '" + std::to_string(generalName->type);
                    }
                }
                sk_GENERAL_NAME_pop_free(subjectAltNames, GENERAL_NAME_free);

                X509_free(server_cert);
            } else {
                VLOG(0) << "     Server certificate: no certificate";
            }
        },
        [](tls::SocketClient<rfcomm::Socket>::SocketConnection* socketConnection) -> void { // onDisconnect
            VLOG(0) << "OnDisconnect";
            VLOG(0) << "\tServer: " + socketConnection->getRemoteAddress().address() + "(" +
                           socketConnection->getRemoteAddress().address() +
                           "):" + std::to_string(socketConnection->getRemoteAddress().channel());
            VLOG(0) << "\tClient: " + socketConnection->getLocalAddress().address() + "(" + socketConnection->getLocalAddress().address() +
                           "):" + std::to_string(socketConnection->getLocalAddress().channel());
        },
        [](tls::SocketClient<rfcomm::Socket>::SocketConnection* socketConnection, const char* junk, ssize_t junkSize) -> void { // onRead
            std::string data(junk, junkSize);
            VLOG(0) << "Data to reflect: " << data;
            socketConnection->enqueue(data);
            socketConnection->close();
        },
        []([[maybe_unused]] tls::SocketClient<rfcomm::Socket>::SocketConnection* socketConnection, int errnum) -> void { // onReadError
            VLOG(0) << "OnReadError: " << errnum;
        },
        []([[maybe_unused]] tls::SocketClient<rfcomm::Socket>::SocketConnection* socketConnection, int errnum) -> void { // onWriteError
            VLOG(0) << "OnWriteError: " << errnum;
        },
        {{"certChain", CLIENTCERTF}, {"keyPEM", CLIENTKEYF}, {"password", KEYFPASS}, {"caFile", SERVERCAFILE}});

    return btClient;
}

int main(int argc, char* argv[]) {
    net::EventLoop::init(argc, argv);

    {
        RfCommAddress remoteAddress("5C:C5:D4:B8:3C:AA", 1); // calisto
        RfCommAddress bindAddress("44:01:BB:A3:63:32");      // mpow

        tls::SocketClient legacyClient = getBtClient();

        legacyClient.connect(
            remoteAddress,
            [](int err) -> void {
                if (err) {
                    PLOG(ERROR) << "Connect: " << std::to_string(err);
                } else {
                    VLOG(0) << "Connected";
                }
            },
            bindAddress);
    }

    return net::EventLoop::start();
}