#ifndef INC_H
# define INC_H

// ========================================================================= //
//                                   Header                                  //
// ========================================================================= //

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <errno.h>


// ========================================================================= //
//                                   Define                                  //
// ========================================================================= //

#define FLAG_D 0x01 << 0
#define FLAG_F 0x01 << 1
#define FLAG_M 0x01 << 2
#define FLAG_N 0x01 << 3
#define FLAG_Q 0x01 << 4
#define FLAG_W 0x01 << 5

#define TRACEROUTE_DEFAULT_TIMEOUT      5
#define TRACEROUTE_DEFAULT_FIRST_TTL    1
#define TRACEROUTE_DEFAULT_MAX_TTL      30
#define TRACEROUTE_DEFAULT_NQUERIES     3
#define PACKET_SIZE                     60
#define MAX_PACKET_SIZE                 1024


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
    u32     opt_v_first_ttl;
    u32     opt_v_max_ttl;
    u32     opt_v_nqueries;
    u32     opt_v_timeout;
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
bool socket_initialization(t_data *data);
void options_initialization(t_options *option);

/* utils */
u16     checksum(void *b, int len);
void    print_man();
bool    manage_flags(t_data *data, i32 ac, char **av);
double  calcul_latency(timeval start_time, timeval end_time);
void    print_line(t_data *data, u16 ttl, t_time times[]);
void    close_sockfd_and_exit(t_data *data);
bool    ip_to_hostname(char *ip, char *res);

/* libc */
void	*mem_set(void *s, int c, size_t n);
char    *str_cpy(char *dest, const char *src);
int     str_cmp(const char *s1, const char *s2);
int     m_atoi(const char *str);
int     is_digit(int c);
size_t	str_len(const char *s);


/* debug */
void print_sent_packet(t_packet *packet);
void print_received_packet(char *response);


#endif /* INC_H */