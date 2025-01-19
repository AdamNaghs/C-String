#ifndef _N_STR_H
#define _N_STR_H

#include <string.h>

typedef struct
{
    char *cstr; /* heap allocated */
    int len;
} n_str;

n_str n_str_new(const char *str);
n_str n_str_n_new(const char *str, size_t n);
void n_str_free(n_str *str);
void n_str_append(n_str *dest, n_str *append);
void n_str_n_append(n_str *dest, n_str *append, unsigned int n);
void n_str_append_cstr(n_str *dest, const char *append);
void n_str_n_append_cstr(n_str *dest, const char *append, int n);
void n_str_append_arr(n_str *dest_str, n_str *append_arr, int len, n_str *sep);
n_str n_str_copy(n_str *str);
n_str n_str_copy_append(n_str *from, n_str *back);
n_str n_str_copy_append_cstr(n_str *from, const char *back);
n_str *n_str_split(char **patterns, size_t patterns_len, int include,
                   n_str *source, size_t *ret_arr_len);
void n_str_arr_free(n_str *, int len);
/* set replace_count to 0 to replace all */
n_str n_str_replace(unsigned int replace_count, n_str *str,
                    n_str *sub_str, n_str *new_str);
n_str n_read_file(const char *file_name);

#define N_STR_FREE_ALL(...)                                                                                                   \
    do                                                                                                                        \
    {                                                                                                                         \
        n_str *_N_STR_FREE_ALL_arr[] = {__VA_ARGS__};                                                                         \
        int _N_STR_FREE_ALL_i;                                                                                                \
        for (_N_STR_FREE_ALL_i = 0; _N_STR_FREE_ALL_i < (sizeof(_N_STR_FREE_ALL_arr) / sizeof(n_str *)); _N_STR_FREE_ALL_i++) \
            n_str_free(_N_STR_FREE_ALL_arr[_N_STR_FREE_ALL_i]);                                                               \
    } while (0)

#endif /*_N_STR_H */