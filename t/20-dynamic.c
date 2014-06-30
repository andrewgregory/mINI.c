#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MINI_BUFFER_SIZE 2

#include "mini.c"

#include "tap.c/tap.c"

int main(int argc, char **argv)
{
    char buf[] =
        "\n"
        " a \n"
        " aa \n"
        " a ###################################\n"
        " aaa \n"
        " a \n"
        "\n";
    FILE *stream = fmemopen(buf, sizeof(buf), "r");
    mini_t *ini = mini_finit(stream);

    tap_plan(15);

    tap_ok(ini != NULL, "mini_init");

    tap_is_int(ini->_buf_size, 2, NULL);

    mini_next(ini);
    tap_is_str(ini->key, "a", "a - key");
    tap_is_int(ini->_buf_size, 6, "a - size");

    mini_next(ini);
    tap_is_str(ini->key, "aa", "aa - key");
    tap_is_int(ini->_buf_size, 6, "aa - size");

    mini_next(ini);
    tap_is_str(ini->key, "a", "a - key");
    tap_is_int(ini->_buf_size, 6, "a - size");

    mini_next(ini);
    tap_is_str(ini->key, "aaa", "aaa - key");
    tap_is_int(ini->_buf_size, 8, "aaa - size");

    mini_next(ini);
    tap_is_str(ini->key, "a", "a - key");
    tap_is_int(ini->_buf_size, 8, "a - size");

    tap_ok(mini_next(ini) == NULL, NULL);
    tap_ok(feof(ini->stream), NULL);
    tap_ok(ini->eof, NULL);

    mini_free(ini);

    return 0;
}
