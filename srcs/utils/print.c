#include "inc.h"

void print_man(void)
{
    printf(
        "Usage:\n"
        "   ft_traceroute [options] <destination>\n"
        "Options:\n"
        "   -d                 debug mode\n"
        "   -? -h --help       print manual\n"
    );
}

void    print_line(u16 ttl, t_time times[3])
{
    fprintf(stdout, "%d  ", ttl);

    for (u8 i = 0; i < 3; i++) {
        if (times[i].end_time.tv_sec == 0 && times[i].end_time.tv_usec == 0) {
            fprintf(stdout, "* ");
        } else {
            if (i == 0 || strcmp(times[i].src_ip, times[i - 1].src_ip))
                fprintf(stdout, "%s (%s)  ", times[i].src_hostname, times[i].src_ip);
            fprintf(stdout, "%.3f ms  ", calcul_latency(times[i].start_time, times[i].end_time));
        }
    }
    fprintf(stdout, "\n");
}
