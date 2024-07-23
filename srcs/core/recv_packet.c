#include "inc.h"

void print_received_packet(iphdr *ip_header, icmphdr *icmp_header, char *response) {
    printf("[Packet Received]\n");
    
    printf("[IP Header]\n");
    printf(" |-Version     : %d\n", ip_header->version);
    printf(" |-IHL         : %d (bytes)\n", ip_header->ihl * 4);
    printf(" |-TOS         : %d\n", ip_header->tos);
    printf(" |-Total Length: %d\n", ntohs(ip_header->tot_len));
    printf(" |-ID          : %d\n", ntohs(ip_header->id));
    printf(" |-Frag Offset : %d\n", ntohs(ip_header->frag_off) & 0x1FFF);
    printf(" |-TTL         : %d\n", ip_header->ttl);
    printf(" |-Protocol    : %d\n", ip_header->protocol);
    printf(" |-Checksum    : %d\n", ntohs(ip_header->check));
    printf(" |-Source IP   : %s\n", __ip_str(ip_header->saddr));
    printf(" |-Dest IP     : %s\n", __ip_str(ip_header->daddr));

    printf("[ICMP Header]\n");
    printf(" |-Type        : %d\n", icmp_header->type);
    printf(" |-Code        : %d\n", icmp_header->code);
    printf(" |-Checksum    : %d\n", icmp_header->checksum);
    printf(" |-Identifier  : %d\n", icmp_header->un.echo.id);
    printf(" |-Sequence    : %d\n", icmp_header->un.echo.sequence);

    printf("[Message Data]\n");
    printf(" |-Size        : %lu bytes\n", strlen(response + sizeof(*ip_header) + sizeof(*icmp_header)) + 1);
    printf(" |-Content     : %s\n", response + sizeof(*ip_header) + sizeof(*icmp_header));
    printf("------------------------------------------------------------------------------\n\n");
}

void recv_packet(t_data *data, char *response, timeval *end_time, u16 *n_sequence)
{
    if (recvfrom(data->sockfd, response, PACKET_SIZE, 0, NULL, NULL) < 0) {
        return ;
    }
    gettimeofday(end_time, NULL);
    ++*n_sequence;


        if (data->flags & FLAG_D) {
            iphdr *ip_hdr = (iphdr *)response;
            icmphdr *icmp_hdr = (icmphdr *)(response + (ip_hdr->ihl * 4));

            print_received_packet(ip_hdr, icmp_hdr, response);
        }
}
