#include "inc.h"

static void traceroute(t_data *data)
{
    t_packet packet;
    char response[MAX_PACKET_SIZE];
    u16 n_sequence;
    t_time times[3];
    iphdr *ip_hdr;

    memset(response, 0, sizeof(response));

    printf("traceroute to %s (%s), %d hops max, %d byte packets\n",
                data->addr_in, data->addr, data->option.option_max_ttl_value, PACKET_SIZE);

    for (u8 ttl = data->option.option_first_ttl_value; ttl <= data->option.option_max_ttl_value; ttl++) {
        setsockopt(data->sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
        memset(times, 0, sizeof(times));
        n_sequence = 0;

        for (u8 i = 0; i < 3; i++) {
            prepare_packet(data, &packet, n_sequence);
            send_packet(data, &packet, &times[i].start_time, &n_sequence);
            recv_packet(data, response, &times[i].end_time, &n_sequence);
            
            ip_hdr = (iphdr *)response;
            
            char hostname_response_sender[NI_MAXHOST];
            if (!ip_to_hostname(__ip_str(ip_hdr->saddr), hostname_response_sender))
                close_sockfd_and_exit(data);

            strcpy(times[i].src_ip, __ip_str(ip_hdr->saddr));
            strcpy(times[i].src_hostname, hostname_response_sender);
        }
        print_line(ttl, times);

        if (!strcmp(times[0].src_ip, data->addr)) {
            break ;
        }
    }
}

int main(int ac, char **av)
{
    t_data data;
    memset(&data, 0, sizeof(data));
    
    data.addr_in = av[ac - 1];
    data.option.option_first_ttl_value = TRACEROUTE_DEFAULT_FIRST_TTL;
    data.option.option_max_ttl_value = TRACEROUTE_DEFAULT_MAX_TTL;
    
    av++, ac--;

    if (!ac) {
        print_man();
        return EXIT_FAILURE;
    }

    if (!manage_flags(&data, ac, av) || !reverse_dns(data.addr_in, data.addr))
        return EXIT_FAILURE;

    if (!initialization(&data))
        close_sockfd_and_exit(&data);

    traceroute(&data);
    close(data.sockfd);

    return EXIT_SUCCESS;
}
