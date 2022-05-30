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

unsigned char config_exists()
{
    struct stat stats;

    stat(_CPM_CONF_FILE, &stats);
    return S_ISREG(stats.st_mode) > 0;
}

unsigned char is_complete_config(Config *config)
{
    unsigned char i = 0;
    Dependency *dependency = NULL;

    if (!config->name)
        return 0;
    if (!config->description)
        return 0;
    if (!config->version)
        return 0;
    if (!config->author)
        return 0;
    while (i < _CPM_DEP_LIM && (dependency = config->dependencies[i++]))
    {
        if (!dependency->name)
            return 0;
        if (!dependency->version)
            return 0;
    }
    return 1;
}

unsigned char dependencies_length_config(Config *config)
{
    unsigned char i = 0;
    Dependency *dependency = NULL;

    while (i < _CPM_DEP_LIM && (dependency = config->dependencies[i]))
        if (!dependency->name || !dependency->version)
            return i;
        else
            i++;
    return i;
}

// Function to create the cpm.conf file. As the same way as npm does, this file
// will contain the project's name, description, author, version and most of
// all, dependencies. Also create the modules folder. This location will
// contain all installed dependencies from the cpm.conf file.
unsigned char init_config()
{
    FILE *fp = NULL;

    if (!module_folder_exists())
        mkdir(_CPM_MOD_FOLDER, _CPM_PERMS);
    if (!module_folder_exists())
    {
        perror(_CPM_PERM_ERR);
        return 1;
    }
    if (config_exists())
        return 0;
    fp = fopen(_CPM_CONF_FILE, "w+");
    if (fp == NULL)
    {
        perror(_CPM_PERM_ERR);
        return 1;
    }
    fwrite(_CPM_DEF_CONF, 1, strlen(_CPM_DEF_CONF), fp);
    fclose(fp);
    if (!config_exists())
    {
        perror(_CPM_PERM_ERR);
        return 1;
    }
    return 0;
}

// Function to display an existing config in the console.
void show_config(Config *config)
{
    unsigned int i = 0;
    Dependency *dependency = NULL;

    if (!is_complete_config(config))
    {
        perror(_CPM_MIS_CONF_ERR);
        return;
    }
    printf("Name : %s\n", config->name);
    printf("Description : %s\n", config->description);
    printf("Author : %s\n", config->author);
    printf("Version : %s\n", config->version);
    while (i < _CPM_DEP_LIM && (dependency = config->dependencies[i++]))
        printf("Dependency n°%d : %s==%s\n", i, dependency->name, dependency->version);
}

// Function to free the config the cleanest way possible.
void destroy_config(Config *config)
{
    unsigned char i = 0;
    Dependency *dependency = NULL;

    free(config->name);
    free(config->description);
    free(config->author);
    free(config->version);
    while (i < _CPM_DEP_LIM && (dependency = config->dependencies[i++]))
    {
        free(dependency->name);
        free(dependency->version);
        free(dependency);
    }
    free(config->dependencies);
    free(config);
}

Config *new_config(char const *path)
{
    Config *config = (Config *)malloc(sizeof(Config));
    char *raw_config = read_cpm_conf(path);
    unsigned char errno = 0;

    if (!config)
    {
        free(raw_config);
        return NULL;
    }
    config->name = NULL;
    config->description = NULL;
    config->version = NULL;
    config->author = NULL;
    config->dependencies = (Dependency **)calloc(_CPM_DEP_LIM, sizeof(Dependency *));
    errno = parse_config(raw_config, config);
    free(raw_config);
    if (!is_complete_config(config))
    {
        destroy_config(config);
        perror(_CPM_MIS_CONF_ERR);
        return NULL;
    }
    if (errno > 0)
    {
        destroy_config(config);
        return NULL;
    }
    return config;
}
size_t evaluate_config_length(Config *config)
{
    unsigned char i = 0;
    size_t length = strlen(_CPM_DEF_CONF);
    Dependency *dependency = NULL;

    while (i < _CPM_DEP_LIM && (dependency = config->dependencies[i++]))
        length += strlen(dependency->name) + strlen(dependency->version) + strlen("=\"\"\n");
    return length;
}

char *get_raw_config(Config *config)
{
    size_t length = 0;
    unsigned char i = 0;
    char *raw_config = NULL;
    Dependency *dependency = NULL;

    if (!is_complete_config(config))
    {
        free(raw_config);
        perror(_CPM_MIS_CONF_ERR);
        return NULL;
    }
    length = evaluate_config_length(config);
    raw_config = (char *)malloc(sizeof(char) * length);
    if (!raw_config)
        return NULL;
    sprintf(raw_config, _CPM_DEF_CONF, config->name, config->description, config->version, config->author);
    if (dependencies_length_config(config) == 0)
        return raw_config;
    while (i < _CPM_DEP_LIM && (dependency = config->dependencies[i++]))
    {
        if (!dependency->name || !dependency->version)
            break;
        strcat(raw_config, dependency->name);
        strcat(raw_config, "=\"");
        strcat(raw_config, dependency->version);
        strcat(raw_config, "\"\n");
    }
    return raw_config;
}

void write_config(Config *config)
{
    char *raw_config = get_raw_config(config);
    write_cpm_conf(_CPM_CONF_FILE, (char const *)raw_config);
    free(raw_config);
}
