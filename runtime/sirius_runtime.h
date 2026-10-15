#ifndef SIRIUS_RUNTIME_H
#define SIRIUS_RUNTIME_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef int64_t S_i64;
typedef double S_f64;
typedef bool S_bool;

typedef struct { bool has_value; int64_t value; } S_Optional_i64;

typedef struct S_Array_i64 {
    size_t refcount;
    size_t length;
    size_t capacity;
    int64_t data[];
} S_Array_i64;

typedef struct S_String {
    size_t refcount;
    size_t length;
    char data[];
} S_String;

S_Array_i64 *s_array_i64_make(size_t n, const int64_t *values);
S_Array_i64 *s_array_i64_retain(S_Array_i64 *arr);
void s_array_i64_release(S_Array_i64 *arr);
S_Optional_i64 s_array_i64_get(const S_Array_i64 *arr, int64_t index);
int64_t s_array_i64_get_checked(const S_Array_i64 *arr, int64_t index);
void s_array_i64_set(S_Array_i64 **arr, int64_t index, int64_t value);
S_Array_i64 *s_array_i64_append_take(S_Array_i64 *owned, int64_t item);
int64_t s_array_i64_length_take(S_Array_i64 *owned);
int64_t s_array_i64_get_checked_take(S_Array_i64 *owned, int64_t index);

typedef struct { bool has_value; S_String *value; } S_Optional_string;

typedef struct S_Dict_i64_i64 S_Dict_i64_i64;
S_Dict_i64_i64 *s_dict_i64_i64_new(void);
S_Dict_i64_i64 *s_dict_i64_i64_retain(S_Dict_i64_i64 *dict);
void s_dict_i64_i64_release(S_Dict_i64_i64 *dict);
S_Dict_i64_i64 *s_dict_i64_i64_insert(S_Dict_i64_i64 *dict, int64_t key, int64_t value);
S_Optional_i64 s_dict_i64_i64_get(const S_Dict_i64_i64 *dict, int64_t key);

S_Optional_string s_read_text(const S_String *path);
bool s_write_text(const S_String *path, const S_String *text);

S_String *s_string_from_utf8(const char *str);
S_String *s_string_from_bytes(const char *bytes, size_t n);
S_String *s_string_retain(S_String *str);
void s_string_release(S_String *str);
S_String *s_string_concat(const S_String *a, const S_String *b);
void s_print_string(const S_String *str);
S_String *s_string_concat_take(S_String *a, S_String *b);
int s_string_compare_take(S_String *a, S_String *b);
int64_t s_string_length_take(S_String *s);
void s_print_string_take(S_String *s);
void s_print_i64(int64_t v);
void s_print_f64(double v);
void s_print_bool(bool v);
void s_eprint_string(const S_String *str);
void s_eprint_string_take(S_String *str);
void s_eprint_i64(int64_t v);
void s_eprint_f64(double v);
void s_eprint_bool(bool v);
_Noreturn void s_panic(const char *reason);

/* Generic, type-specialized-by-the-compiler bootstrap collections.  The
   callbacks retain/drop embedded managed values; NULL denotes trivial data. */
typedef void (*S_CopyElem)(void *, const void *);
typedef void (*S_DropElem)(void *);
typedef uint64_t (*S_HashKey)(const void *);
typedef bool (*S_EqualKey)(const void *, const void *);
typedef struct S_Array S_Array;
typedef struct S_Dict S_Dict;
S_Array *s_array_new(size_t size, S_CopyElem copy, S_DropElem drop);
S_Array *s_array_make(size_t n, const void *data, size_t size, S_CopyElem copy, S_DropElem drop);
S_Array *s_array_retain(S_Array *a);
void s_array_release(S_Array *a);
size_t s_array_length(const S_Array *a);
const void *s_array_at(const S_Array *a, int64_t i);
S_Array *s_array_append(S_Array *owned, const void *item);
void s_array_set(S_Array **owned, int64_t index, const void *item);
S_Dict *s_dict_new(size_t ksize, size_t vsize, S_CopyElem kcopy, S_DropElem kdrop,
                   S_CopyElem vcopy, S_DropElem vdrop, S_HashKey hash, S_EqualKey eq);
S_Dict *s_dict_retain(S_Dict *d);
void s_dict_release(S_Dict *d);
S_Dict *s_dict_insert(S_Dict *owned, const void *key, const void *value);
const void *s_dict_get(const S_Dict *d, const void *key);
size_t s_dict_length(const S_Dict *d);
uint64_t s_hash_i64_key(const void *key);
bool s_eq_i64_key(const void *a, const void *b);
uint64_t s_hash_string_key(const void *key);
bool s_eq_string_key(const void *a, const void *b);
int s_string_compare(const S_String *a, const S_String *b);
size_t s_string_length(const S_String *s);
S_Optional_i64 s_string_byte_at(const S_String *s, int64_t i);
S_String *s_string_slice(const S_String *s, int64_t start, int64_t end);
S_Optional_i64 s_string_find(const S_String *s, const S_String *needle);
bool s_string_starts_with(const S_String *s, const S_String *prefix);
bool s_string_ends_with(const S_String *s, const S_String *suffix);
S_String *s_string_from_i64(int64_t value);
S_String *s_string_join(const S_String *separator, const S_Array *parts);
/* Take ownership of arguments passed by the Sirius C emitter. */
S_Optional_i64 s_string_byte_at_take(S_String *s, int64_t index);
S_String *s_string_slice_take(S_String *s, int64_t start, int64_t end);
S_Optional_i64 s_string_find_take(S_String *s, S_String *needle);
bool s_string_starts_with_take(S_String *s, S_String *prefix);
bool s_string_ends_with_take(S_String *s, S_String *suffix);
S_String *s_string_join_take(S_String *separator, S_Array *parts);
S_Optional_string s_read_text_take(S_String *path);
bool s_write_text_take(S_String *path, S_String *text);
void s_set_arguments(int argc, char **argv);
S_Array *s_arguments(void);

#endif /* SIRIUS_RUNTIME_H */
