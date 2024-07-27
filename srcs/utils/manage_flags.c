#include "inc.h"

static bool set_option_value(t_data *data, i32 index, i32 ac, char **av, u8 flag)
{
    if (!(index + 1 < ac)) {
        fprintf(stderr, "Option `%s' (argc %d) requires an argument\n", av[index], index + 1);
        return false;
    }

    u8 len = str_len(av[++index]);
    for (u8 i = 0; i < len; i++) {
        if (!isdigit(av[index][i])) {
            fprintf(stderr, "Cannot handle `%s' option with arg `%s' (argc %d)\n", av[index - 1], av[index], index + 1);
            return false;
        }
    }

    if (!str_cmp(av[index - 1], "-f")) {
        data->option.opt_v_first_ttl = m_atoi(av[index]);
    } else if (!str_cmp(av[index - 1], "-w")) {
        data->option.opt_v_timeout = m_atoi(av[index]);
    } else if (!str_cmp(av[index - 1], "-m")) {
        data->option.opt_v_max_ttl = m_atoi(av[index]);

        if (data->option.opt_v_max_ttl > 255) {
            fprintf(stderr, "max hops cannot be more than 255\n");
            return false;
        }
    } else if (!str_cmp(av[index - 1], "-q")) {
        data->option.opt_v_nqueries = m_atoi(av[index]);
        
        if (data->option.opt_v_nqueries > 10) {
            fprintf(stderr, "no more than 10 probes per hop\n");
            return false;
        } 
    }
    
    data->flags |= flag;
    return true;
}

bool manage_flags(t_data *data, i32 ac, char **av)
{
    for (i32 index = 0; index < ac; index++) {
        
        if (!str_cmp(av[index], "-?") || !str_cmp(av[index], "-h") || !str_cmp(av[index], "--help")) {
            print_man();
            exit(EXIT_SUCCESS);
        } else if (!str_cmp(av[index], "-d") || !str_cmp(av[index], "--debug")) {
            data->flags |= FLAG_D;
        } else if (!str_cmp(av[index], "-n")) {
            data->flags |= FLAG_N;
        } else if (!str_cmp(av[index], "-f") && !set_option_value(data, index, ac, av, FLAG_F)) {
            return false;
        } else if (!str_cmp(av[index], "-m") && !set_option_value(data, index, ac, av, FLAG_M)) {
            return false;
        } else if (!str_cmp(av[index], "-q") && !set_option_value(data, index, ac, av, FLAG_Q)) {
            return false;
        } else if (!str_cmp(av[index], "-w") && !set_option_value(data, index, ac, av, FLAG_W)) {
            return false;
        }
    }

    if (data->option.opt_v_first_ttl > data->option.opt_v_max_ttl) {
        fprintf(stderr, "first hop out of range\n");
        return false;
    }
    return true;
}
