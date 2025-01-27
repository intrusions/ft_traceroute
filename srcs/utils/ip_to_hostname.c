#include "inc.h"

bool ip_to_hostname(char *ip, char *res)
{
    sockaddr_in sa;
    char host[NI_MAXHOST];

    mem_set(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;

    if (inet_pton(AF_INET, ip, &(sa.sin_addr)) <= 0) {
        __log_error("inet_pton error");
        return false;
    }

    if (getnameinfo((sockaddr*)&sa, sizeof(sa), host, sizeof(host), NULL, 0, 0) != 0) {
        str_cpy(res, ip);
    } else {
        str_cpy(res, host);
    }

    return true;
}
