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

unsigned char set_config_info(Config *config, char *field)
{
    Set *set = new_set(field);
    unsigned char success = 1;

    if (!set)
        return !success;
    if (strcmp(set->key, "name") == 0 && !config->name)
    {
        config->name = (char *)malloc(sizeof(char) * strlen(set->value));
        strcpy(config->name, set->value);
    }
    else if (strcmp(set->key, "description") == 0 && !config->description)
    {
        config->description = (char *)malloc(sizeof(char) * strlen(set->value));
        strcpy(config->description, set->value);
    }
    else if (strcmp(set->key, "version") == 0 && !config->version)
    {
        config->version = (char *)malloc(sizeof(char) * strlen(set->value));
        strcpy(config->version, set->value);
    }
    else if (strcmp(set->key, "author") == 0 && !config->author)
    {
        config->author = (char *)malloc(sizeof(char) * strlen(set->value));
        strcpy(config->author, set->value);
    }
    else
        success = 0;
    destroy_set(set);
    return success;
}

unsigned char set_config_dependency(Config *config, char *field, unsigned char dependency_index)
{
    Set *set = new_set(field);
    Dependency *dependency = (Dependency *)malloc(sizeof(Dependency));

    if (dependency_index == _CPM_DEP_LIM)
    {
        perror(_CPM_MAX_DEP_ERR);
        free(dependency);
        return 0;
    }
    if (!set)
    {
        free(dependency);
        return 0;
    }
    if (!dependency)
        return 0;
    dependency->name = (char *)malloc(sizeof(char) * strlen(set->key));
    strcpy(dependency->name, set->key);
    dependency->version = (char *)malloc(sizeof(char) * strlen(set->value));
    strcpy(dependency->version, set->value);
    config->dependencies[dependency_index] = dependency;
    destroy_set(set);
    return 1;
}

char *reset_buffer(char *buffer, size_t length)
{
    for (size_t i = 0; i < length; i++)
        buffer[i] = 0;
    return buffer;
}

size_t longest_line_length(char *str)
{
    size_t h = 0;
    size_t c = 0;

    for (size_t i = 0; i < strlen(str); i++)
        if (str[i] == '\n')
        {
            if (c > h)
                h = c;
            c = 0;
        }
        else
            c++;
    return (c > h) ? c : h;
}

unsigned char parse_config(char *raw_config, Config *config)
{
    int buffer_i = 0;
    size_t max_buffer_length = longest_line_length(raw_config);
    char *buffer = (char *)malloc(sizeof(char) * max_buffer_length);
    Section current_section = NONE;
    unsigned char c;
    unsigned char is_reading_section_name = 0;
    unsigned char dependencies_counter = 0;

    for (size_t i = 0; i < strlen(raw_config) + 1; i++)
    {
        c = raw_config[i];
        if (c == '\n' || c == 0)
        {
            if (buffer_i == 0)
                continue;
            buffer_i = 0;
            if (is_valid_section(buffer))
            {
                buffer = reset_buffer(buffer, max_buffer_length);
                continue;
            }
            if (current_section == INFO)
                if (!set_config_info(config, buffer))
                {
                    printf("PARSE ERROR [INFO] : %s\n", buffer);
                    free(buffer);
                    return 1;
                }
            if (current_section == DEPENDENCIES)
                if (!set_config_dependency(config, buffer, dependencies_counter++))
                {
                    printf("PARSE ERROR [DEPENDENCIES] : %s\n", buffer);
                    free(buffer);
                    return 2;
                }
            buffer = reset_buffer(buffer, max_buffer_length);
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
                printf(_CPM_UNK_SEC_ERR, buffer);
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
