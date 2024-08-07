#include "inc.h"

static bool open_sockfd(t_data *data)
{
    data->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    return (!(data->sockfd < 0));
}

bool socket_initialization(t_data *data)
{
    if (!open_sockfd(data)) {
        __log_error("socket error");
        return false;
    }

    data->pid = getpid();

    mem_set(&data->dest, 0, sizeof(data->dest));
    data->dest.sin_family = AF_INET;
    
    if (inet_pton(AF_INET, data->addr, &data->dest.sin_addr) <= 0) {
        __log_error("inet_pton error");
        return false;
    }

    timeval tv = {data->option.opt_v_timeout, 0};
    if (setsockopt(data->sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) != 0) {
        __log_error("setsockopt error");
        return false;
    }
    return true;
}
