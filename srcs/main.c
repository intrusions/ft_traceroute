#include "inc.h"

static bool initialization(t_data *data)
{
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

    struct timeval tv = {TIMEOUT, 0};
    if (setsockopt(data->sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) != 0) {
        close(data->sockfd);
        fprintf(stderr, "error setting socket options\n");
        return true;
    }
    return false;
}

static void traceroute(t_data *data)
{
    t_packet packet;
    char response[PACKET_SIZE];
    u16 n_sequence;
    t_time times[3];
    struct iphdr *ip_hdr;

    memset(response, 0, sizeof(response));

    printf("traceroute to %s (%s), %d hops max, %d byte packets\n", data->hostname_in, data->hostname, MAX_HOPS, PACKET_SIZE);
    for (u8 ttl = 1; ttl <= MAX_HOPS; ttl++) {
        setsockopt(data->sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
        memset(times, 0, sizeof(times));
        n_sequence = 0;

        for (u8 i = 0; i < 3; i++) {
            prepare_packet(data, &packet, n_sequence);
            send_packet(data, &packet, &times[i].start_time, &n_sequence);
            recv_packet(data, response, &times[i].end_time, &n_sequence);
            
            ip_hdr = (struct iphdr *)response;
            strcpy(times[i].src_ip, inet_ntoa(*(struct in_addr *)&ip_hdr->saddr));
        }
        print_line(ttl, times);

        if (!strcmp(times[0].src_ip, data->hostname)) {
            break ;
        }
    }
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