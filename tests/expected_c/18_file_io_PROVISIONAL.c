/* Reference lowering, not required byte-for-byte compiler output. */
#include "sirius_runtime.h"
int main(void) {
    S_String *path = s_string_from_utf8("example.txt");
    S_String *message = s_string_from_utf8("Hello, Sirius!");
    s_print_bool(s_write_text(path, message));
    S_Optional_string maybe = s_read_text(path);
    S_String *fallback = NULL;
    S_String *content;
    if (maybe.has_value) content = maybe.value;
    else { fallback = s_string_from_utf8("missing"); content = fallback; }
    s_print_string(content);
    s_string_release(content); /* owns read result OR fallback, never both */
    s_string_release(message);
    s_string_release(path);
    return 0;
}
