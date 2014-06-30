#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "mini.c"

#include "tap.c/tap.c"

int main(int argc, char **argv)
{
    char buf[] =
        "\n"
        " key outside section\n"
        " [ section with spaces ] \n"
        " [key \n"
        " key = value \n"
        " key with spaces \n"
        "key=value\n"
        "key#this is a comment\n"
        " [] # empty section name\n"
        "emptysection\n";
    FILE *stream = fmemopen(buf, sizeof(buf), "r");
    mini_t *ini = mini_finit(stream);

    tap_plan(58);

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

    tap_ok(mini_next(ini) != NULL, NULL);
    tap_is_int(ini->lineno, 9, NULL);
    tap_is_str(ini->section, "", NULL);
    tap_is_str(ini->key, NULL, NULL);
    tap_is_str(ini->value, NULL, NULL);
    tap_is_int(ini->eof, 0, NULL);

    tap_ok(mini_next(ini) != NULL, NULL);
    tap_is_int(ini->lineno, 10, NULL);
    tap_is_str(ini->section, "", NULL);
    tap_is_str(ini->key, "emptysection", NULL);
    tap_is_str(ini->value, NULL, NULL);
    tap_is_int(ini->eof, 0, NULL);

    tap_ok(mini_next(ini) == NULL, NULL);
    tap_ok(feof(ini->stream), NULL);
    tap_ok(ini->eof, NULL);

    mini_free(ini);

    return 0;
}
