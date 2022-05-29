#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../include/error.h"
#include "../include/config.h"
#include "../include/parser.h"
#include "../include/util.h"

unsigned char module_folder_exists()
{
    struct stat stats;

    stat(_CPM_MOD_FOLDER, &stats);
    return S_ISDIR(stats.st_mode) > 0;
}

unsigned char conf_exists()
{
    struct stat stats;

    stat(_CPM_CONF_FILE, &stats);
    return S_ISREG(stats.st_mode) > 0;
}

// Function to create the cpm.conf file.
// As the same way as npm does, this file
// will contain the project's name,
// description, author, version and most of
// all, dependencies.
// Also create the modules folder.
// This location will contain all installed
// dependencies from the cpm.conf file.
unsigned char init()
{
    FILE *fp = NULL;

    if (!module_folder_exists())
        mkdir(_CPM_MOD_FOLDER, _CPM_PERMS);
    if (!module_folder_exists())
    {
        perror(_CPM_PERM_ERR);
        return 1;
    }
    if (conf_exists())
        return 0;
    fp = fopen(_CPM_CONF_FILE, "w+");
    if (fp == NULL)
    {
        perror(_CPM_PERM_ERR);
        return 1;
    }
    fwrite(DEFAULT_CONFIG, 1, strlen(DEFAULT_CONFIG), fp);
    fclose(fp);
    if (!conf_exists())
    {
        perror(_CPM_PERM_ERR);
        return 1;
    }
    return 0;
}

// Function to display an existing config
// in the console.
void show_config(Config *config)
{
    int i = 0;
    Dependency *d = NULL;

    if (config->name)
        printf("Name : %s\n", config->name);
    if (config->description)
        printf("Description : %s\n", config->description);
    if (config->author)
        printf("Author : %s\n", config->author);
    if (config->version)
        printf("Version : %s\n", config->version);
    while (i < DEPENDENCIES_LIMIT && (d = config->dependencies[i++]))
        printf("Dependency n°%d : %s==%s\n", i, d->name, d->version);
}

// Function to free the config the cleanest
// way possible.
void destroy_config(Config *config)
{
    int i = 0;
    Dependency *d = NULL;

    free(config->name);
    free(config->description);
    free(config->author);
    free(config->version);
    while (i < DEPENDENCIES_LIMIT && (d = config->dependencies[i++]))
    {
        free(d->name);
        free(d->version);
        free(d);
    }
    free(config->dependencies);
    free(config);
}

Config *new_config(char const *path)
{
    Config *config = (Config *)malloc(sizeof(Config));
    char *raw_config = read_cpm_conf(path);
    unsigned char result = 0;

    if (!config)
    {
        free(raw_config);
        return NULL;
    }
    config->name = NULL;
    config->description = NULL;
    config->version = NULL;
    config->author = NULL;
    config->dependencies = (Dependency **)calloc(DEPENDENCIES_LIMIT, sizeof(Dependency *));
    result = parse_cpm_config(raw_config, config);
    free(raw_config);
    if (result != 0)
    {
        destroy_config(config);
        return NULL;
    }
    return config;
}
