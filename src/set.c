#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/set.h"

Set *new_set(char *str)
{
    size_t str_length = strlen(str);
    Set *set = (Set *)malloc(sizeof(Set));
    unsigned char c;
    unsigned char reading_key = 0;
    size_t key_length = 0;
    size_t value_length = 0;
    size_t l = 0;
    if (!set)
        return NULL;
    for (size_t i = 0; i < str_length; i++)
    {
        c = str[i];
        if (c == '=')
        {
            if (!set->key && !set->value)
            {
                key_length = sizeof(char) * (i + 1);
                value_length = sizeof(char) * (str_length - key_length + 1);
                set->key = (char *)malloc(key_length);
                set->value = (char *)malloc(value_length);
                if (!set->key || !set->value)
                    return NULL;
                i = -1;
                reading_key = 1;
                continue;
            }
            reading_key = 0;
            set->key[l] = 0;
            l = 0;
            continue;
        }
        if (reading_key && set->key)
            set->key[l++] = c;
        if (!reading_key && set->value)
            set->value[l++] = c;
    }
    set->value[l] = 0;
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
