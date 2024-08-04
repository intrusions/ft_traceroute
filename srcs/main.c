#include "inc.h"

static void traceroute(t_data *data)
{
    t_packet packet;
    char response[MAX_PACKET_SIZE];
    u16 n_sequence;
    t_packet_response_info pri[data->option.opt_v_nqueries];
    iphdr *ip_hdr;

    mem_set(response, 0, sizeof(response));

    printf("traceroute to %s (%s), %d hops max, %d byte packets\n",
                data->addr_in, data->addr, data->option.opt_v_max_ttl, PACKET_SIZE);

    for (u8 ttl = data->option.opt_v_first_ttl; ttl <= data->option.opt_v_max_ttl; ttl++) {
        setsockopt(data->sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
        mem_set(pri, 0, sizeof(pri));
        n_sequence = 0;

        for (u8 i = 0; i < data->option.opt_v_nqueries; i++) {
            prepare_packet(data, &packet, n_sequence);
            send_packet(data, &packet, &pri[i].start_time, &n_sequence);
            recv_packet(data, response, &pri[i].end_time);
            
            ip_hdr = (iphdr *)response;
            
            char hostname_response_sender[NI_MAXHOST];
            if (!ip_to_hostname(__ip_str(ip_hdr->saddr), hostname_response_sender))
                close_sockfd_and_exit(data);

            str_cpy(pri[i].src_ip, __ip_str(ip_hdr->saddr));
            str_cpy(pri[i].src_hostname, hostname_response_sender);
        }
        print_line(data, ttl, pri);

        if (!str_cmp(pri[0].src_ip, data->addr)) {
            break ;
        }
    }
}

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
