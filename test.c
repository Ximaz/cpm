#include <stdio.h>
#include <stdlib.h>
#include "./include/config.h"

int main(void)
{
    // init_modules_folder();
    Config *conf = get_cpm_conf("cpm.example.conf");
    show_conf(conf);
    free_config(conf);
    return 0;
}
