#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "../include/error.h"
#include "../include/config.h"
#include "../include/parser.h"
#include "../include/util.h"

unsigned char chk_mod_folder_stat()
{
    struct stat stats;

    stat(_CPM_MOD_FOLDER, &stats);
    return S_ISDIR(stats.st_mode) > 0;
}

unsigned char chk_conf_stat()
{
    struct stat stats;

    stat(_CPM_CONF_FILE, &stats);
    return S_ISREG(stats.st_mode) > 0;
}

unsigned char init_modules_folder()
{
    if (!chk_mod_folder_stat())
        mkdir(_CPM_MOD_FOLDER, _CPM_PERMS);
    if (!chk_mod_folder_stat())
    {
        printf(_CPM_MOD_FOLDER_NOT_CREATED, _CPM_MOD_FOLDER);
        return 1;
    }
    return 0;
}

unsigned char init_cpm_conf()
{
    FILE *fp = NULL;

    if (chk_conf_stat())
        return 0;
    fp = fopen(_CPM_CONF_FILE, "w+");
    if (fp == NULL)
    {
        printf(_CPM_CONF_FILE_NOT_CREATED, _CPM_CONF_FILE);
        return 1;
    }
    fclose(fp);
    return 0;
}

void show_conf(Config *conf)
{
    int i = 0;
    Dependency *d = NULL;

    printf("Name : %s\n", conf->name);
    printf("Description : %s\n", conf->description);
    printf("Author : %s\n", conf->author);
    printf("Version : %s\n", conf->version);
    while ((d = conf->dependencies[i++]))
    {
        printf("Dep %d : %s - %s\n", i, d->name, d->version);
    }
}

void free_config(Config *conf)
{
    int i = 0;
    Dependency *d = NULL;

    free(conf->name);
    free(conf->description);
    free(conf->author);
    free(conf->version);
    while ((d = conf->dependencies[i++]))
    {
        free(d->name);
        free(d->version);
        free(d);
    }
    free(conf->dependencies);
    free(conf);
}

Config *get_cpm_conf(char const *path)
{
    Config *conf = (Config *)malloc(sizeof(Config));
    char *raw_conf = read_cpm_conf(path);
    unsigned char result = 0;

    if (!conf)
    {
        free(raw_conf);
        printf("%s", _CPM_CONF_STRUCT_MEM);
        return NULL;
    }
    conf->dependencies = (Dependency **) calloc(1, sizeof(Dependency *));
    result = parse_cpm_conf(raw_conf, conf);
    free(raw_conf);
    if (result != 0)
    {
        free_config(conf);
        return NULL;
    }
    return conf;
}
