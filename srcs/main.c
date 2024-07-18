#include "inc.h"

static bool initialization(t_data *data)
{
    struct timeval tv = {TIMEOUT, 0};

    if ((data->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
        fprintf(stderr, "socket creation failed\n");
        return true;
    }

    data->pid = getpid();

    memset(&data->dest, 0, sizeof(data->dest));
    data->dest.sin_family = AF_INET;
    
    if (inet_pton(AF_INET, data->hostname, &data->dest.sin_addr) <= 0) {
        close(data->sockfd);
        fprintf(stderr, "invalid address\n");
        return true;
    }

    if (setsockopt(data->sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) != 0) {
        close(data->sockfd);
        fprintf(stderr, "error setting socket options\n");
        return true;
    }
    return false;
}

static void traceroute(t_data *data)
{
    (void)data;
}

int main(int ac, char **av)
{
    t_data data;
    memset(&data, 0, sizeof(data));
    data.hostname_in = av[ac - 1];
    
    av++, ac--;

    if (!ac) {
        print_man();
        return EXIT_FAILURE;
    }
    if (manage_flags(ac, av, &data.flags)) {
        return EXIT_SUCCESS;
    }
    if (reverse_dns(data.hostname_in, data.hostname)) {
        return EXIT_FAILURE;
    }
    if (initialization(&data)) {
        close(data.sockfd);
        return EXIT_FAILURE;
    }

    traceroute(&data);
    close(data.sockfd);

    return EXIT_SUCCESS;
}