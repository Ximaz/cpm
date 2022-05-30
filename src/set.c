#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/set.h"

void get_key(char *str, Set *set)
{
    size_t i = 0;
    unsigned char c;
    char *key = (char *)malloc(sizeof(char) * strlen(str));

    if (!key)
        return;
    while ((c = str[i]) && c != '=' && c != '\n')
        key[i++] = c;
    if (*key != 0)
        key[i] = 0;
    set->key = (char *) malloc(sizeof(char) * i);
    strcpy(set->key, key);
    free(key);
}

void get_value(char *str, Set *set)
{
    size_t i = 0;
    size_t j = 0;
    unsigned char c;
    unsigned char reading_value = 0;
    char *value = (char *)malloc(sizeof(char) * strlen(str));

    if (!value)
        return;
    while ((c = str[i++]) && !reading_value)
        if (c == '=')
            reading_value = 1;
    if (!reading_value)
        free(value);
    i--;
    while ((c = str[i++]))
        value[j++] = c;
    if (*value != 0)
        value[j] = 0;
    if (DELIM_QUOTES && value[0] == '"' && value[j - 1] == '"')
    {
        i = 1;
        for (; i < j - 1; i++)
            value[i - 1] = value[i];
        --i;
        value[i] = 0;
        j = i;
    }
    set->value = (char *) malloc(sizeof(char) * j);
    strcpy(set->value, value);
    free(value);
}

Set *new_set(char *str)
{
    Set *set = (Set *)malloc(sizeof(Set));

    if (!set)
        return NULL;
    get_key(str, set);
    get_value(str, set);
    return set;
}

void destroy_set(Set *set)
{
    free(set->key);
    free(set->value);
    free(set);
}

void display_set(Set *set)
{
    if (set->key)
        printf("%s", set->key);
    printf("=");
    if (set->value)
        printf("%s", set->value);
    printf("\n");
}
