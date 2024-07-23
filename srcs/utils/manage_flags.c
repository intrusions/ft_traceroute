#include "inc.h"

bool manage_flags(i32 ac, char **av, u8 *flags)
{
    for (i32 index = 0; index < ac - 1; index++) {
        
        if (!strcmp(av[index], "-?") || !strcmp(av[index], "-h") || !strcmp(av[index], "--help")) {
            print_man();
            exit(EXIT_SUCCESS);
        } else if (!strcmp(av[index], "-d") || !strcmp(av[index], "--debug")) {
            *flags |= FLAG_D;
        } else {
            print_man();
            return false;
        }
    }
    return true;
}
