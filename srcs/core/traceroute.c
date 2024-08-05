#include "inc.h"

static bool fill_response_info_struct(t_packet_response_info *pri, iphdr *ip_hdr, u8 index)
{
    char hostname_response_sender[NI_MAXHOST];
    if (!ip_to_hostname(__ip_str(ip_hdr->saddr), hostname_response_sender))
        return false;

    str_cpy(pri[index].src_ip, __ip_str(ip_hdr->saddr));
    str_cpy(pri[index].src_hostname, hostname_response_sender);
    
    return true;
}

void traceroute(t_data *data)
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

        for (u8 index = 0; index < data->option.opt_v_nqueries; index++) {
            prepare_packet(data, &packet, n_sequence);
            send_packet(data, &packet, &pri[index].start_time, &n_sequence);
            recv_packet(data, response, &pri[index].end_time);
            
            ip_hdr = (iphdr *)response;
            if (!fill_response_info_struct(pri, ip_hdr, index))
                close_sockfd_and_exit(data);
        }
        print_line(data, pri);

        if (!str_cmp(pri[0].src_ip, data->addr))
            break ;
    }
}