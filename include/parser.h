#ifndef _CPM_PARSER_H_
#define _CPM_PARSER_H_

typedef struct s_dependency
{
    char *name;
    char *version;
} Dependency;

typedef struct s_config
{
    char *name;
    char *description;
    char *version;
    char *author;
    Dependency **dependencies;
} Config;

typedef enum e_section
{
    INFO = 1,
    DEPENDENCIES = 2,
    NONE = 0
} Section;

unsigned char parse_config(char *raw_config, Config *config);

#endif // _CPM_PARSER_H_
