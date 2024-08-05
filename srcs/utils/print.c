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
        "   -w                 Set the time (in seconds) to wait for a response to a probe (default 5.0 sec).\n"
        "   -f                 Specifies with what TTL to start. Defaults to 1. \n"
        "   -? -h --help       Print help info and exit. \n"
    );
}

void    print_line(t_data *data, t_packet_response_info pri[])
{
    static u8 ttl = 0;

    fprintf(stdout, "%d  ", ++ttl);

    for (u8 i = 0; i < data->option.opt_v_nqueries; i++) {
        if (pri[i].end_time.tv_sec == 0 && pri[i].end_time.tv_usec == 0) {
            fprintf(stdout, "* ");
        } else {
            if (i == 0 || str_cmp(pri[i].src_ip, pri[i - 1].src_ip)) {
                
                if (data->flags & FLAG_N)
                    fprintf(stdout, "%s  ", pri[i].src_ip);
                else
                    fprintf(stdout, "%s (%s)  ", pri[i].src_hostname, pri[i].src_ip);

            }
            fprintf(stdout, "%.3f ms  ", calcul_latency(pri[i].start_time, pri[i].end_time));
        }
    }
    fprintf(stdout, "\n");
}
