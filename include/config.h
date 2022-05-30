#ifndef _CPM_CONF_H_
#define _CPM_CONF_H_

#include "parser.h"

// Limit of allowed dependencies for a project.
#define _CPM_DEP_LIM 64

// Example of configuration. Will be used at the init.
#define _CPM_DEF_CONF "[INFO]\nname=\"%s\"\ndescription=\"%s\"\nauthor=\"%s\"\nversion=\"%s\"\n\n[DEPENDENCIES]\n"

unsigned char init_config();
void show_config(Config *config);
void destroy_config(Config *config);
Config *new_config(char const *path);
unsigned char is_complete(Config *config);
unsigned char dependencies_length_config(Config *config);
char *get_raw_config(Config *config);
void write_config(Config *config);

#endif // _CPM_CONF_H_
