#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "mini.c"

#include "tap.c/tap.c"

int main(int argc, char **argv)
{
    char temp_path[] = "mini_test_XXXXXX";
    int fd1 = mkstemp(temp_path);
    FILE *stream = fdopen(fd1, "r+");

    fputs(
            "\n"
            " key outside section\n"
            " [ section with spaces ] \n"
            " [key \n"
            " key = value \n"
            " key with spaces \n"
            "key=value\n"
            "key#this is a comment\n",
            stream);
    fclose(stream);

    mini_t *ini = mini_init(temp_path);

    tap_plan(46);

    tap_ok(ini != NULL, "mini_init");

    tap_ok(mini_next(ini) != NULL, NULL);
    tap_is_int(ini->lineno, 2, NULL);
    tap_is_str(ini->section, NULL, NULL);
    tap_is_str(ini->key, "key outside section", NULL);
    tap_is_str(ini->value, NULL, NULL);
    tap_is_int(ini->eof, 0, NULL);

    tap_ok(mini_next(ini) != NULL, NULL);
    tap_is_int(ini->lineno, 3, NULL);
    tap_is_str(ini->section, " section with spaces ", NULL);
    tap_is_str(ini->key, NULL, NULL);
    tap_is_str(ini->value, NULL, NULL);
    tap_is_int(ini->eof, 0, NULL);

    tap_ok(mini_next(ini) != NULL, NULL);
    tap_is_int(ini->lineno, 4, NULL);
    tap_is_str(ini->section, " section with spaces ", NULL);
    tap_is_str(ini->key, "[key", NULL);
    tap_is_str(ini->value, NULL, NULL);
    tap_is_int(ini->eof, 0, NULL);

    tap_ok(mini_next(ini) != NULL, NULL);
    tap_is_int(ini->lineno, 5, NULL);
    tap_is_str(ini->section, " section with spaces ", NULL);
    tap_is_str(ini->key, "key", NULL);
    tap_is_str(ini->value, "value", NULL);
    tap_is_int(ini->eof, 0, NULL);

    tap_ok(mini_next(ini) != NULL, NULL);
    tap_is_int(ini->lineno, 6, NULL);
    tap_is_str(ini->section, " section with spaces ", NULL);
    tap_is_str(ini->key, "key with spaces", NULL);
    tap_is_str(ini->value, NULL, NULL);
    tap_is_int(ini->eof, 0, NULL);

    tap_ok(mini_next(ini) != NULL, NULL);
    tap_is_int(ini->lineno, 7, NULL);
    tap_is_str(ini->section, " section with spaces ", NULL);
    tap_is_str(ini->key, "key", NULL);
    tap_is_str(ini->value, "value", NULL);
    tap_is_int(ini->eof, 0, NULL);

    tap_ok(mini_next(ini) != NULL, NULL);
    tap_is_int(ini->lineno, 8, NULL);
    tap_is_str(ini->section, " section with spaces ", NULL);
    tap_is_str(ini->key, "key", NULL);
    tap_is_str(ini->value, NULL, NULL);
    tap_is_int(ini->eof, 0, NULL);

    tap_ok(mini_next(ini) == NULL, NULL);
    tap_ok(feof(ini->stream), NULL);
    tap_ok(ini->eof, NULL);

    mini_free(ini);
    unlink(temp_path);

    return 0;
}
