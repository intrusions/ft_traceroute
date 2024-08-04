#include "inc.h"

void recv_packet(t_data *data, char *response, timeval *end_time)
{
    if (recvfrom(data->sockfd, response, MAX_PACKET_SIZE, 0, NULL, NULL) < 0) {
        return ;
    }
    gettimeofday(end_time, NULL);

    if (data->flags & FLAG_D)
        print_received_packet(response);
}
