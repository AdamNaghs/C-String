#include "../Testing/ctf.h"
#include "str.h"

static n_str new_n_str, n_new_n_str;

CTF_TEST(new)
{
    char *tmp = "Hello, world!";
    n_str ret = new_n_str = n_str_new(tmp);
    CTF_ASSERT(strcmp(tmp, ret.cstr) == 0);
    CTF_PASS();
}

CTF_TEST(n_new)
{
    char *tmp = "Hello, world!";
    n_str ret = n_new_n_str = n_str_n_new(tmp, 5);
    CTF_ASSERT(strncmp(tmp, ret.cstr, 5) == 0);
    CTF_PASS();
}

CTF_TEST(free)
{
    n_str_free(&new_n_str);
    n_str_free(&n_new_n_str);
    CTF_ASSERT(new_n_str.len == 0);
    CTF_ASSERT(new_n_str.cstr == NULL);
    CTF_ASSERT(n_new_n_str.len == 0);
    CTF_ASSERT(n_new_n_str.cstr == NULL);
    CTF_PASS();
}

CTF_TEST(append)
{
    n_str a = n_str_new("Hello, ");
    n_str b = n_str_new("world!");
    n_str_append(&a, &b);
    CTF_ASSERT(strcmp(a.cstr, "Hello, world!") == 0);
    n_str_free(&a);
    n_str_free(&b);
    CTF_PASS();
}

CTF_TEST(copy)
{
    n_str a = n_str_new("Hello, world!");
    n_str b = n_str_copy(&a);
    CTF_ASSERT(strcmp(a.cstr, b.cstr) == 0);
    n_str_free(&a);
    n_str_free(&b);
    CTF_PASS();
}

CTF_TEST(copy_append)
{
    n_str a = n_str_new("Hello, ");
    n_str b = n_str_new("world!");
    n_str c = n_str_copy_append(&a, &b);
    CTF_ASSERT(strcmp(c.cstr, "Hello, world!") == 0);
    n_str_free(&a);
    n_str_free(&b);
    n_str_free(&c);
    CTF_PASS();
}

CTF_TEST(split)
{
    // Test basic splitting
    n_str source = n_str_new("Hello,world!,How,are,you");
    char *patterns[] = {","};
    size_t ret_len;
    n_str *splits = n_str_split(patterns, 1, 0, &source, &ret_len);

    // Check number of splits
    CTF_ASSERT(ret_len == 5);

    // Check split contents
    CTF_ASSERT(strcmp(splits[0].cstr, "Hello") == 0);
    CTF_ASSERT(strcmp(splits[1].cstr, "world!") == 0);
    CTF_ASSERT(strcmp(splits[2].cstr, "How") == 0);
    CTF_ASSERT(strcmp(splits[3].cstr, "are") == 0);
    CTF_ASSERT(strcmp(splits[4].cstr, "you") == 0);

    // Test with include flag
    size_t incl_ret_len;
    n_str *incl_splits = n_str_split(patterns, 1, 1, &source, &incl_ret_len);

    // Check splits with delimiter included
    CTF_ASSERT(incl_ret_len == 5);
    CTF_ASSERT(strcmp(incl_splits[0].cstr, "Hello,") == 0);
    CTF_ASSERT(strcmp(incl_splits[1].cstr, "world!,") == 0);
    CTF_ASSERT(strcmp(incl_splits[2].cstr, "How,") == 0);
    CTF_ASSERT(strcmp(incl_splits[3].cstr, "are,") == 0);
    CTF_ASSERT(strcmp(incl_splits[4].cstr, "you") == 0);

    // Test multiple patterns
    char *multi_patterns[] = {",", "!"};
    size_t multi_ret_len;
    n_str *multi_splits = n_str_split(multi_patterns, 2, 0, &source, &multi_ret_len);

    // Check multi-pattern splits
    CTF_ASSERT_CLEAN_LOG(multi_ret_len == 5, {
        for (int i = 0; i < multi_ret_len; i ++)
        {
            printf("%d:%s\n",i,multi_splits[i].cstr);
        } }, "Actual: %zu", multi_ret_len);
    CTF_ASSERT(strcmp(multi_splits[0].cstr, "Hello") == 0);
    CTF_ASSERT(strcmp(multi_splits[1].cstr, "world") == 0);
    CTF_ASSERT_LOG(strcmp(multi_splits[2].cstr, "How") == 0, "Actual: '%s'", multi_splits[2].cstr);
    CTF_ASSERT(strcmp(multi_splits[3].cstr, "are") == 0);
    CTF_ASSERT(strcmp(multi_splits[4].cstr, "you") == 0);

    // Clean up
    n_str_arr_free(splits, ret_len);
    n_str_arr_free(incl_splits, incl_ret_len);
    n_str_arr_free(multi_splits, multi_ret_len);
    n_str_free(&source);

    CTF_PASS();
}

CTF_TEST(replace)
{
    // Basic replacement
    n_str str = n_str_new("Hello world! Hello universe!");
    n_str sub = n_str_new("Hello");
    n_str new_str = n_str_new("Hi");

    // Replace all occurrences
    n_str result = n_str_replace(0, &str, &sub, &new_str);
    CTF_ASSERT_LOG(strcmp(result.cstr, "Hi world! Hi universe!") == 0,
                   "Expected 'Hi world! Hi universe!', got '%s'", result.cstr);

    // Replace only first occurrence
    n_str result2 = n_str_replace(1, &str, &sub, &new_str);
    CTF_ASSERT_LOG(strcmp(result2.cstr, "Hi world! Hello universe!") == 0,
                   "Expected 'Hi world! Hello universe!', got '%s'", result2.cstr);

    // Test replacement at end of n_str
    n_str str2 = n_str_new("Hello world Hello");
    n_str result3 = n_str_replace(0, &str2, &sub, &new_str);
    CTF_ASSERT_LOG(strcmp(result3.cstr, "Hi world Hi") == 0,
                   "Expected 'Hi world Hi', got '%s'", result3.cstr);

    // Clean up
    n_str_free(&str);
    n_str_free(&sub);
    n_str_free(&new_str);
    n_str_free(&str2);
    n_str_free(&result);
    n_str_free(&result2);
    n_str_free(&result3);

    CTF_PASS();
}

CTF_SUITE(
    n_str,
    {
        CTF_LINK(n_str, new);
        CTF_LINK(n_str, n_new);
        CTF_LINK(n_str, free);
        CTF_LINK(n_str, append);
        CTF_LINK(n_str, copy);
        CTF_LINK(n_str, copy_append);
        CTF_LINK(n_str, split);
        CTF_LINK(n_str, replace);
    })

void main1(int argc, char **argv)
{
    CTF_PROCESS_INIT();
    CTF_SUITE_RUN(n_str);
    CTF_PROCESS_EXIT();
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage:\n\t%s: %s [filename]\n", *argv, *argv);
        return 0;
    }
    n_str file = n_read_file(argv[1]);
    size_t lines_len = 0;
    char *patterns[] = {"\n"};
    n_str *lines = n_str_split(patterns, 1, 0, &file, &lines_len);
    n_str tmp = n_str_new(""), sep = n_str_new("EOL\n");
    n_str_append_arr(&tmp, lines, lines_len, &sep);
    n_str_append_cstr(&tmp, "EOF");
    printf("%s\n", tmp.cstr);
    n_str_arr_free(lines, lines_len);
    N_STR_FREE_ALL(&tmp, &sep, &file);
    return 0;
}