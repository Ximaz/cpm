#ifndef _CPM_SET_H_
#define _CPM_SET_H_

typedef struct s_set
{
    char *key;
    char *value;
} Set;

Set *new_set(char *str);
void destroy_set(Set *set);
void display_set(Set *set);

#endif // _CPM_SET_H_
