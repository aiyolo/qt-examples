#include "mdns.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <iphlpapi.h>
#else
#include <ifaddrs.h>
#include <net/if.h>
#endif


static int service_callback(int sock, const struct sockaddr* from, size_t addrlen,
                           mdns_entry_type_t entry, uint16_t query_id, uint16_t rtype,
                           uint16_t rclass, uint32_t ttl, const void* data, size_t size,
                           size_t name_offset, size_t name_length, size_t record_offset,
                           size_t record_length, void* user_data) {

    char name_buffer[256];

    printf("Service discovery response: %d\n", rtype);

    if (rtype == MDNS_RECORDTYPE_PTR) {
        mdns_string_t name = mdns_record_parse_ptr(data, size, record_offset, record_length,
                                                   name_buffer, sizeof(name_buffer));

        if (name.length > 0) {
            printf("Found service type: %.*s\n", (int)name.length, name.str);
        }
    }

    return 0;
}

int main() {
    printf("Starting mDNS service discovery...\n");

#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
        printf("Failed to initialize WinSock\n");
        return -1;
    }
#endif

    // Open socket for mDNS discovery
    int sock = mdns_socket_open_ipv4(NULL);
    if (sock < 0) {
        printf("Failed to open mDNS socket\n");
#ifdef _WIN32
        WSACleanup();
#endif
        return -1;
    }

    printf("mDNS socket opened successfully\n");

    // Send discovery request
    if (mdns_discovery_send(sock)) {
        printf("Failed to send discovery request\n");
        mdns_socket_close(sock);
#ifdef _WIN32
        WSACleanup();
#endif
        return -1;
    }

    printf("Discovery request sent, waiting for responses...\n\n");

    // Buffer for receiving data
    char buffer[1024 * 16];

    // Receive and parse responses multiple times to catch all services
    for (int i = 0; i < 5; i++) {
        size_t records = mdns_discovery_recv(sock, buffer, sizeof(buffer), service_callback, NULL);
        if (records > 0) {
            printf("Received %zu records in iteration %d\n", records, i + 1);
        }

        // Small delay to allow more responses
#ifdef _WIN32
        Sleep(200);
#else
        usleep(200000);
#endif
    }

    printf("\nService discovery completed.\n");
    // Sleep(100000);
    // Close socket and cleanup
    mdns_socket_close(sock);
#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
