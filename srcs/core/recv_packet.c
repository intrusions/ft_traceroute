#include "inc.h"

void recv_packet(t_data *data, char *response, timeval *end_time, u16 *n_sequence)
{
    if (recvfrom(data->sockfd, response, MAX_PACKET_SIZE, 0, NULL, NULL) < 0) {
        return ;
    }
    gettimeofday(end_time, NULL);
    ++*n_sequence;

    if (data->flags & FLAG_D)
        print_received_packet(response);
}
