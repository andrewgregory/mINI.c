#include <stdio.h>

#include "tap.c/tap.c"

#include "mini.c"

/* just verify the library compiles */

int main() {
    tap_plan(1);
    tap_ok(1, "compilation successful");
    return 0;
}
