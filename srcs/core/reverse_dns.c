#include "inc.h"

bool    reverse_dns(char *addr_in, char *addr)
{
    addrinfo hints;
    addrinfo *addr_info, *ptr;

    mem_set(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(addr_in, NULL, &hints, &addr_info) != 0) {
        __log_error("getaddrinfo error");
        return false;
    }

    for (ptr = addr_info; ptr; ptr = ptr->ai_next) {

        if (ptr->ai_family == AF_INET) {
            sockaddr_in *ipv4 = (sockaddr_in *)ptr->ai_addr;
            void *curr_addr = inet_ntoa(ipv4->sin_addr);
            
            if (!curr_addr) {
                __log_error("inet_ntoa error");
                freeaddrinfo(addr_info);
                return false;
            }

            str_cpy(addr, curr_addr);
            break ;
        }
    }

    freeaddrinfo(addr_info);
    return true;
}
