#include "str.h"
#include <stdlib.h>
#include <stdio.h>

n_str n_str_new(const char *str)
{
    size_t len = strlen(str);
    n_str ret = {malloc(len + 1), len};

    if (!ret.cstr)
    {
        ret.len = 0;
        return ret;
    }

    memcpy(ret.cstr, str, len);
    ret.cstr[len] = 0;
    return ret;
}

n_str n_str_n_new(const char *str, size_t n)
{
    n_str ret = {malloc(n + 1), n};

    if (!ret.cstr)
    {
        ret.len = 0;
        return ret;
    }

    memcpy(ret.cstr, str, n);
    ret.cstr[n] = 0;
    return ret;
}

void n_str_free(n_str *str)
{
    if (str && str->cstr)
    {
        free(str->cstr);
        str->cstr = NULL;
        str->len = 0;
    }
}

void n_str_append(n_str *dest, n_str *append)
{
    if (!dest || !append || !append->cstr)
        return;

    char *new_data = realloc(dest->cstr, dest->len + append->len + 1);
    if (!new_data)
        return;

    dest->cstr = new_data;
    memcpy(dest->cstr + dest->len, append->cstr, append->len);
    dest->len += append->len;
    dest->cstr[dest->len] = 0;
}

void n_str_n_append(n_str *dest, n_str *append, unsigned int n)
{
    if (!dest || !append || !append->cstr)
        return;

    char *new_data = realloc(dest->cstr, dest->len + n + 1);
    if (!new_data)
        return;

    dest->cstr = new_data;
    memcpy(dest->cstr + dest->len, append->cstr, n);
    dest->len += append->len;
    dest->cstr[dest->len] = 0;
}

n_str n_str_copy(n_str *str)
{
    if (!str || !str->cstr)
    {
        return (n_str){NULL, 0};
    }
    return n_str_n_new(str->cstr, str->len);
}

n_str n_str_copy_append(n_str *from, n_str *back)
{
    if (!from || !back)
    {
        return (n_str){NULL, 0};
    }

    n_str ret = n_str_copy(from);
    if (ret.cstr)
    {
        n_str_append(&ret, back);
    }
    return ret;
}

n_str *n_str_split(char **patterns, size_t patterns_len, int include,
                   n_str *source, size_t *ret_arr_len)
{
    if (!patterns || !source || !source->cstr || !ret_arr_len)
    {
        *ret_arr_len = 0;
        return NULL;
    }

    size_t cap = 10;
    size_t size = 0;
    n_str *ret = calloc(cap, sizeof(n_str));
    if (!ret)
    {
        *ret_arr_len = 0;
        return NULL;
    }

    size_t last = 0;
    for (size_t i = 0; i < source->len;)
    {
        int found_pattern = 0;

        for (size_t p = 0; p < patterns_len; p++)
        {
            const char *pattern = patterns[p];
            size_t len = strlen(pattern);

            if (i + len <= source->len &&
                strncmp(source->cstr + i, pattern, len) == 0)
            {

                /*  Only create a new n_str if there's content before the pattern */
                if (i > last || include)
                {
                    n_str tmp = n_str_n_new(source->cstr + last,
                                            include ? (i + len - last) : (i - last));

                    if (!tmp.cstr)
                    {
                        for (size_t j = 0; j < size; j++)
                        {
                            n_str_free(&ret[j]);
                        }
                        free(ret);
                        *ret_arr_len = 0;
                        return NULL;
                    }

                    if (size >= cap)
                    {
                        size_t new_cap = cap * 2;
                        n_str *new_ret = realloc(ret, new_cap * sizeof(n_str));
                        if (!new_ret)
                        {
                            for (size_t j = 0; j < size; j++)
                            {
                                n_str_free(&ret[j]);
                            }
                            free(ret);
                            n_str_free(&tmp);
                            *ret_arr_len = 0;
                            return NULL;
                        }
                        ret = new_ret;
                        cap = new_cap;
                    }

                    ret[size++] = tmp;
                }

                i += len;
                last = i;
                found_pattern = 1;
                break;
            }
        }

        if (!found_pattern)
        {
            i++;
        }
    }

    /*  Add remaining n_str if any */
    if (last < source->len)
    {
        n_str tmp = n_str_n_new(source->cstr + last, source->len - last);
        if (!tmp.cstr)
        {
            for (size_t j = 0; j < size; j++)
            {
                n_str_free(&ret[j]);
            }
            free(ret);
            *ret_arr_len = 0;
            return NULL;
        }

        if (size >= cap)
        {
            n_str *new_ret = realloc(ret, (cap + 1) * sizeof(n_str));
            if (!new_ret)
            {
                for (size_t j = 0; j < size; j++)
                {
                    n_str_free(&ret[j]);
                }
                free(ret);
                n_str_free(&tmp);
                *ret_arr_len = 0;
                return NULL;
            }
            ret = new_ret;
        }
        ret[size++] = tmp;
    }

    *ret_arr_len = size;
    return ret;
}

void n_str_arr_free(n_str *s, int n)
{
    for (int i = 0; i < n; i++)
        n_str_free(&s[i]);
    free(s);
}

void n_str_append_cstr(n_str *dest, const char *append)
{
    if (!dest || !append)
        return;
    int len = strlen(append);
    char *new_data = realloc(dest->cstr, sizeof(char) * (dest->len + len + 1));
    if (!new_data)
        return;

    dest->cstr = new_data;
    memcpy(dest->cstr + dest->len, append, len);
    dest->len += len;
    dest->cstr[dest->len] = 0;
}

void n_str_n_append_cstr(n_str *dest, const char *append, int len)
{
    if (!dest || !append)
        return;
    char *new_data = realloc(dest->cstr, dest->len + len + 1);
    if (!new_data)
        return;

    dest->cstr = new_data;
    memcpy(dest->cstr + dest->len, append, len);
    dest->len += len;
    dest->cstr[dest->len] = 0;
}

n_str n_str_copy_append_cstr(n_str *from, const char *back)
{
    if (!from || !back)
    {
        return (n_str){NULL, 0};
    }

    n_str ret = n_str_copy(from);
    if (ret.cstr)
    {
        n_str_append_cstr(&ret, back);
    }
    return ret;
}

n_str n_str_replace(unsigned int replace_count, n_str *str, n_str *sub_str, n_str *new_str)
{
    if (!str || !sub_str || !new_str || !str->cstr || !sub_str->cstr || !new_str->cstr || sub_str->len == 0)
    {
        return n_str_new("");
    }

    n_str final = n_str_new("");
    size_t last = 0;
    unsigned int replacements = 0;

    for (size_t i = 0; i <= str->len - sub_str->len; i++)
    {
        if (strncmp(str->cstr + i, sub_str->cstr, sub_str->len) == 0)
        {
            if (i > last)
            {
                n_str_n_append_cstr(&final, str->cstr + last, i - last);
            }

            n_str_append(&final, new_str);

            last = i + sub_str->len;
            i = last - 1; /*  -1 because loop will increment */

            replacements++;
            if (replace_count > 0 && replacements >= replace_count)
            {
                break;
            }
        }
    }

    /*  Append any remaining text */
    if (last < str->len)
    {
        n_str_n_append_cstr(&final, str->cstr + last, str->len - last);
    }

    return final;
}

n_str n_read_file(const char *file_name)
{
    n_str ret = {NULL, 0};
    FILE *f = fopen(file_name, "r");
    if (!f)
    {
#ifndef N_RELEASE
        fprintf(stderr, "n_read_file: Failed to read file '%s'", file_name);
#endif
        goto file_fail;
    }
    fseek(f, 0L, SEEK_END);
    long i = ftell(f);
    fseek(f, 0L, SEEK_SET);
    char *buff = malloc(sizeof(char) * (i + 1));
    if (!buff)
    {
#ifndef N_RELEASE
        perror("n_read_file: Failed to allocate memory for file's string.");
#endif
        fclose(f);
        return ret;
    }
    fread(buff, sizeof(char), i, f);
    buff[i] = 0;
    ret = (n_str){buff, i};

    fclose(f);
file_fail:
    return ret;
}

void n_str_append_arr(n_str *dest_str, n_str *append_arr, int arr_len, n_str *sep)
{
    int i, len = dest_str->len;
    for (i = 0; i < arr_len; i++)
    {
        len += append_arr[i].len;
    }
    if (sep)
    {
        len += arr_len * sep->len;
    }
    char *tmp = realloc(dest_str->cstr, sizeof(char) * (len + 1));
    if (!tmp)
    {
        perror("Failed to realloc dest_str->cstr");
        return;
    }
    dest_str->cstr = tmp;
    int end = dest_str->len;
    for (i = 0; i < arr_len; i++)
    {
        for (int j = 0; j < append_arr[i].len; j++)
            dest_str->cstr[end++] = append_arr[i].cstr[j];
        if (sep && i + 1 < arr_len)
            for (int k = 0; k < sep->len; k++)
                dest_str->cstr[end++] = sep->cstr[k];
    }
    dest_str->cstr[end] = 0;
    dest_str->len = end;
}