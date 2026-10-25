/* Exercise UTF-8 filenames, binary data, and rejected embedded NUL paths. */
#include "sirius_runtime.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    S_String *path = s_string_from_utf8("sirius-\xC3\xA9.txt");
    S_String *text = s_string_from_bytes("a\0b\n", 4);
    assert(s_filesystem_error_kind() == 0 && s_filesystem_error_code() == 0);
    assert(s_write_text(path, text));
    assert(s_filesystem_error_kind() == 0 && s_filesystem_error_code() == 0);
    S_Optional_string read = s_read_text(path);
    assert(read.has_value);
    assert(read.value->length == text->length);
    assert(memcmp(read.value->data, text->data, text->length) == 0);
    s_string_release(read.value);
    s_string_release(text);
    s_string_release(path);

    S_String *directory = s_string_from_utf8(".");
    assert(s_path_kind(directory) == 2);
    assert(s_filesystem_error_kind() == 0 && s_filesystem_error_code() == 0);
    s_string_release(directory);

    S_String *missing = s_string_from_utf8("sirius-missing-file.txt");
    assert(s_path_kind(missing) == 0);
    assert(s_filesystem_error_kind() == 1 && s_filesystem_error_code() != 0);
    assert(!s_read_text(missing).has_value);
    assert(s_filesystem_error_kind() == 1 && s_filesystem_error_code() != 0);
    assert(s_list_dir(missing) == NULL);
    assert(s_filesystem_error_kind() == 1 && s_filesystem_error_code() != 0);
    s_string_release(missing);

    S_String *bad = s_string_from_bytes("bad\0file", 8);
    text = s_string_from_utf8("test");
    assert(s_path_kind(bad) == 0);
    assert(s_filesystem_error_kind() == 5 && s_filesystem_error_code() != 0);
    assert(!s_write_text(bad, text));
    assert(s_filesystem_error_kind() == 5 && s_filesystem_error_code() != 0);
    assert(!s_read_text(bad).has_value);
    assert(s_filesystem_error_kind() == 5 && s_filesystem_error_code() != 0);
    assert(s_list_dir(bad) == NULL);
    assert(s_filesystem_error_kind() == 5 && s_filesystem_error_code() != 0);
    s_string_release(text);
    s_string_release(bad);
    /* Successful operations reset both fields, so no stale error can leak. */
    S_String *folder = s_string_from_utf8(".");
    S_Array *names = s_list_dir(folder);
    assert(names != NULL);
    assert(s_filesystem_error_kind() == 0 && s_filesystem_error_code() == 0);
    s_array_release(names);
    s_string_release(folder);
#ifndef _WIN32
    /* Existing regular file used as a directory is not a missing path. */
    S_String *regular = s_string_from_utf8("sirius-regular.txt");
    S_String *contents = s_string_from_utf8("x");
    assert(s_write_text(regular, contents));
    assert(s_path_kind(regular) == 1);
    assert(s_filesystem_error_kind() == 0 && s_filesystem_error_code() == 0);
    assert(s_list_dir(regular) == NULL);
    assert(s_filesystem_error_kind() == 3 && s_filesystem_error_code() != 0);
    assert(remove(regular->data) == 0);
    s_string_release(regular);
    s_string_release(contents);
#endif
    puts("PASS filesystem I/O: UTF-8 paths, binary round-trip, invalid paths");
    return 0;
}
