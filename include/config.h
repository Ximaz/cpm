#ifndef _CPM_CONF_H_
#define _CPM_CONF_H_

#include "parser.h"

unsigned char init_modules_folder();
unsigned char init_cpm_conf();
void show_conf(Config *conf);
void free_config(Config *conf);
Config *get_cpm_conf(char const *path);

#endif // _CPM_CONF_H_
