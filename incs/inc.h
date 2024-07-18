#ifndef INC_H
# define INC_H

// ========================================================================= //
//                                   Header                                  //
// ========================================================================= //

#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <math.h>


// ========================================================================= //
//                                   Define                                  //
// ========================================================================= //

#define FLAG_D 0x01

#define TIMEOUT     1
#define MAX_HOPS    30
#define PACKET_SIZE 60


// ========================================================================= //
//                                  Typedef                                  //
// ========================================================================= //

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;


// ========================================================================= //
//                                  Structure                                //
// ========================================================================= //

typedef struct {
    i32         sockfd;
    u16         pid;
    u8          flags;
    
    char        *hostname_in;
    char        hostname[INET6_ADDRSTRLEN];

    struct sockaddr_in dest;
} t_data;

typedef struct {
    struct icmphdr  hdr;
    char            payload[PACKET_SIZE - sizeof(struct icmphdr)];
} t_packet;

typedef struct {
    struct timeval  start_time;
    struct timeval  end_time;
    char            src_ip[INET6_ADDRSTRLEN];
} t_time;

// ========================================================================= //
//                                  Prototype                                //
// ========================================================================= //


/* core */
bool reverse_dns(char *hostname_in, char *hostname);
void prepare_packet(t_data *data, t_packet *packet, u16 n_sequence);
void send_packet(t_data *data, t_packet *packet, struct timeval *start_time, u16 *n_sequence);
void recv_packet(t_data *data, char *response, struct timeval *end_time, u16 *n_sequence);

/* utils */
u16     checksum(void *b, int len);
void    print_man();
bool    manage_flags(i32 ac, char **av, u8 *flags);
double  calcul_latency(struct timeval start_time, struct timeval end_time);
void    print_line(u16 ttl, t_time times[3]);


/* debug */
void print_sent_packet(t_packet *packet);


#endif /* INC_H */