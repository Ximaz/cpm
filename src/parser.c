#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/set.h"
#include "../include/config.h"
#include "../include/error.h"
#include "../include/parser.h"

unsigned char is_valid_section(char *buffer)
{
    if (strcmp(buffer, "INFO") == 0)
        return 1;
    if (strcmp(buffer, "DEPENDENCIES") == 0)
        return 1;
    return 0;
}

unsigned char set_conf_info(Config *conf, char *field)
{
    Set *set = new_set(field);
    unsigned char success = 1;

    if (!set)
        return !success;
    if (strcmp(set->key, "author") == 0)
        conf->author = set->value;
    else if (strcmp(set->key, "name") == 0)
        conf->name = set->value;
    else if (strcmp(set->key, "description") == 0)
        conf->description = set->value;
    else if (strcmp(set->key, "version") == 0)
        conf->version = set->value;
    else
        success = 0;
    destroy_set(set);
    return success;
}

unsigned char set_conf_dependency(Config *conf, char *field, unsigned char dependency_index)
{
    Set *set = new_set(field);
    if (!set)
        return 0;
    Dependency *dependency = (Dependency *) malloc(sizeof(Dependency));
    dependency->name = set->key;
    dependency->version = set->value;
    conf->dependencies[dependency_index] = dependency;
    destroy_set(set);
    return 1;
}

char *reset_buffer(char *buffer, size_t length)
{
    for (size_t i = 0; i < length; i++)
        buffer[i] = 0;
    return buffer;
}

unsigned char parse_cpm_conf(char *raw_conf, Config *conf)
{
    size_t raw_conf_length = strlen(raw_conf);
    int buffer_i = 0;
    char *buffer = (char *) malloc(sizeof(char) * raw_conf_length);
    Section current_section = NONE;
    unsigned char c;
    unsigned char is_reading_section_name = 0;
    unsigned char dependencies_counter = 0;

    for (size_t i = 0; i < raw_conf_length; i++)
    {
        c = raw_conf[i];
        if (c == '\n')
        {
            buffer_i = 0;
            if (is_valid_section(buffer))
            {
                buffer = reset_buffer(buffer, raw_conf_length);
                continue;
            }
            if (current_section == INFO)
                if (!set_conf_info(conf, buffer))
                {
                    printf("PARSE ERROR [INFO] : %s\n", buffer);
                    free(buffer);
                    return 1;
                }
            if (current_section == DEPENDENCIES)
                if (!set_conf_dependency(conf, buffer, dependencies_counter++))
                {
                    printf("PARSE ERROR [DEPENDENCIES] : %s\n", buffer);
                    free(buffer);
                    return 2;
                }
            buffer = reset_buffer(buffer, raw_conf_length);
        }
        if (c == '[' && buffer_i == 0)
        {
            is_reading_section_name = 1;
            continue;
        }
        if (c == ']' && is_reading_section_name)
        {
            is_reading_section_name = 0;
            if (strcmp(buffer, "INFO") == 0)
                current_section = INFO;
            else if (strcmp(buffer, "DEPENDENCIES") == 0)
                current_section = DEPENDENCIES;
            else
            {
                printf(_CPM_UNKNOWNED_SECTION, buffer);
                free(buffer);
                return 3;
            }
            continue;
        }
        if (c != '\n')
            buffer[buffer_i++] = c;
    }
    free(buffer);
    return 0;
}

// void write_cpm_conf(Config *config, char const *path)
// {
// }
