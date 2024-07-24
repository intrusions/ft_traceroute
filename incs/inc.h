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
#include <errno.h>
#include <ctype.h>


// ========================================================================= //
//                                   Define                                  //
// ========================================================================= //

#define FLAG_D 0x01 << 0
#define FLAG_F 0x01 << 1
#define FLAG_M 0x01 << 2

#define TIMEOUT                         1
#define TRACEROUTE_DEFAULT_FIRST_TTL    1
#define TRACEROUTE_DEFAULT_MAX_TTL      30
#define PACKET_SIZE                     60


// ========================================================================= //
//                                   Macro                                   //
// ========================================================================= //

#define __ip_str(addr) inet_ntoa(*(in_addr *)&(addr))
#define __log_error(error) (void)fprintf(stderr, "%s: %s\n", error, strerror(errno))


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

typedef struct sockaddr_in  sockaddr_in;
typedef struct sockaddr     sockaddr;
typedef struct addrinfo     addrinfo;
typedef struct in_addr      in_addr;
typedef struct icmphdr      icmphdr;
typedef struct iphdr        iphdr;
typedef struct timeval      timeval;

// ========================================================================= //
//                                  Structure                                //
// ========================================================================= //

typedef struct {
    u32     option_first_ttl_value;
    u32     option_max_ttl_value;
}   t_options;

typedef struct {
    i32 sockfd;
    u16 pid;
    u8  flags;
    
    char    *addr_in;
    char    addr[INET6_ADDRSTRLEN];

    t_options   option;
    sockaddr_in dest;
} t_data;

typedef struct {
    icmphdr hdr;
    char    payload[PACKET_SIZE - sizeof(icmphdr)];
} t_packet;

typedef struct {
    timeval start_time;
    timeval end_time;
    
    char    src_ip[INET6_ADDRSTRLEN];
    char    src_hostname[NI_MAXHOST];
} t_time;

// ========================================================================= //
//                                  Prototype                                //
// ========================================================================= //


/* core */
bool reverse_dns(char *addr_in, char *addr);
void prepare_packet(t_data *data, t_packet *packet, u16 n_sequence);
void send_packet(t_data *data, t_packet *packet, timeval *start_time, u16 *n_sequence);
void recv_packet(t_data *data, char *response, timeval *end_time, u16 *n_sequence);

/* utils */
u16     checksum(void *b, int len);
void    print_man();
bool manage_flags(t_data *data, i32 ac, char **av);
double  calcul_latency(timeval start_time, timeval end_time);
void    print_line(u16 ttl, t_time times[3]);
void    close_sockfd_and_exit(t_data *data);
bool    ip_to_hostname(char *ip, char *res);


/* debug */
void print_sent_packet(t_packet *packet);


#endif /* INC_H */