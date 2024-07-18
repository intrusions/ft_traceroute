#include "inc.h"

bool manage_flags(i32 ac, char **av, u8 *flags)
{
    for (i32 index = 0; index < ac; index++) {
        
        if (!strcmp(av[index], "-?") || !strcmp(av[index], "-h") || !strcmp(av[index], "--help")) {
            print_man();
            return true;
        } else if (!strcmp(av[index], "-d") || !strcmp(av[index], "--debug")) {
            *flags |= FLAG_D;
        }
    }
    return false;
}