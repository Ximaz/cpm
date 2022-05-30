#ifndef _CPM_UTIL_H_
#define _CPM_UTIL_H_

#define _CPM_MOD_FOLDER "cpm_modules"
#define _CPM_CONF_FILE "cpm.conf"
#define _CPM_PERMS 0755

size_t get_file_length(FILE *fp);
char *read_cpm_conf(char const *path);
void write_cpm_conf(char const *path, char const *raw_config);

#endif // _CPM_UTIL_H_
