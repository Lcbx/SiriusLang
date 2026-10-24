/* Exercise UTF-8 filenames, binary data, and rejected embedded NUL paths. */
#include "sirius_runtime.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    S_String *path = s_string_from_utf8("sirius-\xC3\xA9.txt");
    S_String *text = s_string_from_bytes("a\0b\n", 4);
    assert(s_write_text(path, text));
    S_Optional_string read = s_read_text(path);
    assert(read.has_value);
    assert(read.value->length == text->length);
    assert(memcmp(read.value->data, text->data, text->length) == 0);
    s_string_release(read.value);
    s_string_release(text);
    s_string_release(path);

    S_String *missing = s_string_from_utf8("sirius-missing-file.txt");
    assert(!s_read_text(missing).has_value);
    s_string_release(missing);

    S_String *bad = s_string_from_bytes("bad\0file", 8);
    text = s_string_from_utf8("test");
    assert(!s_write_text(bad, text));
    assert(!s_read_text(bad).has_value);
    s_string_release(text);
    s_string_release(bad);
    puts("PASS filesystem I/O: UTF-8 paths, binary round-trip, invalid paths");
    return 0;
}
