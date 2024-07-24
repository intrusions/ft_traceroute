#include "inc.h"

void print_man(void)
{
    printf(
        "Usage:\n"
        "   ft_traceroute [options] <destination>\n"
        "Options:\n"
        "   -d                 Debug mode\n"
        "   -n                 Do not try to map IP addresses to host names when displaying them.\n"
        "   -q                 Sets the number of probe packets per hop. The default is 3.\n"
        "   -m                 Specifies the maximum number of hops (max time-to-live value) traceroute will probe. The default is 30.\n"
        "   -f                 Specifies with what TTL to start. Defaults to 1. \n"
        "   -? -h --help       Print help info and exit. \n"
    );
}

void    print_line(t_data *data, u16 ttl, t_time times[])
{
    fprintf(stdout, "%d  ", ttl);

    for (u8 i = 0; i < data->option.option_nqueries_value; i++) {
        if (times[i].end_time.tv_sec == 0 && times[i].end_time.tv_usec == 0) {
            fprintf(stdout, "* ");
        } else {
            if (i == 0 || strcmp(times[i].src_ip, times[i - 1].src_ip)) {
                
                if (data->flags & FLAG_N)
                    fprintf(stdout, "%s  ", times[i].src_ip);
                else
                    fprintf(stdout, "%s (%s)  ", times[i].src_hostname, times[i].src_ip);

            }
            fprintf(stdout, "%.3f ms  ", calcul_latency(times[i].start_time, times[i].end_time));
        }
    }
    fprintf(stdout, "\n");
}
