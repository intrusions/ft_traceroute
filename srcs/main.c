#include "inc.h"

int main(int ac, char **av)
{
    t_data data;
    mem_set(&data, 0, sizeof(data));
    data.addr_in = av[ac - 1];
    options_initialization(&data.option);

    av++, ac--;

    if (!ac) {
        print_man();
        return EXIT_FAILURE;
    }

    if (!manage_flags(&data, ac, av) || !reverse_dns(data.addr_in, data.addr))
        return EXIT_FAILURE;

    if (!socket_initialization(&data))
        close_sockfd_and_exit(&data);

    traceroute(&data);
    close(data.sockfd);

    return EXIT_SUCCESS;
}
