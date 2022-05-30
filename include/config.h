#ifndef _CPM_CONF_H_
#define _CPM_CONF_H_

#include "parser.h"
// Limit of allowed dependencies for a project.
#define DEPENDENCIES_LIMIT 254
// Example of configuration. Will be used at the init.
#define DEFAULT_CONFIG "[INFO]\n\
name=PROJECT_NAME\n\
description=PROJECT_DESCRIPTION\n\
author=PROJECT_AUTHOR\n\
version=PROJECT_VERSION\
\n\
\n\
[DEPENDENCIES]\
\n"

unsigned char init();
void show_config(Config *config);
void destroy_config(Config *config);
Config *new_config(char const *path);

#endif // _CPM_CONF_H_
