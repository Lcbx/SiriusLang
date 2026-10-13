/* Reference lowering, not required byte-for-byte compiler output. */
#include "sirius_runtime.h"
int main(void) {
    S_String *a = s_string_from_utf8("Sun");
    S_String *b = s_string_from_utf8("rise");
    S_String *c = s_string_concat(a, b);
    s_print_string(c);
    s_print_string(a);
    s_string_release(c);
    s_string_release(b);
    s_string_release(a);
    return 0;
}
