#include <stdio.h>
#include <stdlib.h>
#include "../include/util.h"
#include "../include/error.h"

size_t get_file_length(FILE *fp)
{
    size_t length = 0;

    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return length;
}

char *read_cpm_conf(char const *path)
{
    FILE *fp = fopen(path, "r");
    char *buffer = NULL;
    size_t length = 0;

    if (!fp)
    {
        printf(_CPM_CONF_FILE_NOT_FOUND, _CPM_CONF_FILE);
        return NULL;
    }
    length = get_file_length(fp);
    buffer = (char *)malloc(sizeof(char) * length);
    if (!buffer)
    {
        printf(_CPM_CONF_FILE_READING, _CPM_CONF_FILE);
        return NULL;
    }
    if (fread(buffer, 1, length, fp) < length)
    {
        free(buffer);
        printf(_CPM_CONF_FILE_READING_TO_END, _CPM_CONF_FILE);
        return NULL;
    }
    fclose(fp);
    return buffer;
}