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
        perror(_CPM_PERM_ERR);
        return NULL;
    }
    length = get_file_length(fp);
    buffer = (char *)malloc(sizeof(char) * length);
    if (!buffer)
        return NULL;
    if (fread(buffer, 1, length, fp) < length)
    {
        free(buffer);
        return NULL;
    }
    fclose(fp);
    return buffer;
}