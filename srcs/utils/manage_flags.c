#include "inc.h"

static bool set_option_value(t_data *data, i32 index, i32 ac, char **av, u8 flag)
{
    if (!(index + 1 < ac)) {
        fprintf(stderr, "Option `%s' (argc %d) requires an argument\n", av[index], index + 1);
        return false;
    }


    u8 len = strlen(av[++index]);
    for (u8 i = 0; i < len; i++) {
        if (!isdigit(av[index][i])) {
            fprintf(stderr, "Cannot handle `%s' option with arg `%s' (argc %d)\n", av[index - 1], av[index], index + 1);
            return false;
        }
    }


    if (!strcmp(av[index - 1], "-f")) {
        data->option.option_first_ttl_value = atoi(av[index]);
    } else if (!strcmp(av[index - 1], "-m")) {
        data->option.option_max_ttl_value = atoi(av[index]);

        if (data->option.option_max_ttl_value > 255) {
            fprintf(stderr, "max hops cannot be more than 255\n");
            return false;
        }
    }
    
    data->flags |= flag;

    return true;
}

bool manage_flags(t_data *data, i32 ac, char **av)
{
    for (i32 index = 0; index < ac; index++) {
        
        if (!strcmp(av[index], "-?") || !strcmp(av[index], "-h") || !strcmp(av[index], "--help")) {
            print_man();
            exit(EXIT_SUCCESS);
        } else if (!strcmp(av[index], "-d") || !strcmp(av[index], "--debug")) {
            data->flags |= FLAG_D;
        }  else if (!strcmp(av[index], "-f") && !set_option_value(data, index, ac, av, FLAG_F)) {
            return false;
        }  else if (!strcmp(av[index], "-m") && !set_option_value(data, index, ac, av, FLAG_M)) {
            return false;
        }
    }

    if (data->option.option_first_ttl_value > data->option.option_max_ttl_value) {
        fprintf(stderr, "first hop out of range\n");
        return false;
    }
    return true;
}
