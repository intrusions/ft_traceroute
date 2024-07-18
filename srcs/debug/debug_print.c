#include "inc.h"

void print_sent_packet(t_packet *packet)
{
    printf("[Packet sent]\n");
    printf("[ICMP Header]\n");
    printf(" |-Type        : %d\n", packet->hdr.type);
    printf(" |-Code        : %d\n", packet->hdr.code);
    printf(" |-Checksum    : %d\n", packet->hdr.checksum);
    printf(" |-Identifier  : %d\n", packet->hdr.un.echo.id);
    printf(" |-Sequence    : %d\n", packet->hdr.un.echo.sequence);

    printf("[Message Data]\n");
    printf(" |-Size        : %lu bytes\n", sizeof(packet->payload));
    printf(" |-Content     : %s\n", packet->payload);
    printf("------------------------------------------------------------------------------\n\n");
}