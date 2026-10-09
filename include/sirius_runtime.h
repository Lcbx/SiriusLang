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
S_String *s_string_retain(S_String *str);
void s_string_release(S_String *str);
S_String *s_string_concat(const S_String *a, const S_String *b);
void s_print_string(const S_String *str);
void s_print_i64(int64_t v);
void s_print_f64(double v);
void s_print_bool(bool v);
_Noreturn void s_panic(const char *reason);
#endif
