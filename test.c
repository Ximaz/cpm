#include <stdio.h>
#include <stdlib.h>
#include "./include/config.h"

int main(void)
{
    Config *config = new_config("cpm.example.conf");
    if (!config)
        return 1;
    show_config(config);
    destroy_config(config);
    return 0;
}
