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

#define TIMEOUT 1


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


// ========================================================================= //
//                                  Prototype                                //
// ========================================================================= //


/* core */
bool    reverse_dns(char *hostname_in, char *hostname);

/* utils */
u16     checksum(void *b, int len);
void    print_man();
bool    manage_flags(i32 ac, char **av, u8 *flags);

/* debug */



#endif /* INC_H */