#include "sirius_runtime.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

_Noreturn void s_panic(const char *reason) {
    fprintf(stderr, "Sirius panic: %s\n", reason);
    exit(1);
}

S_Array_i64 *s_array_i64_make(size_t n, const int64_t *values) {
    if (n > (SIZE_MAX - sizeof(S_Array_i64)) / sizeof(int64_t)) s_panic("array too large");
    S_Array_i64 *a = malloc(sizeof(*a) + n * sizeof(int64_t));
    if (!a) s_panic("out of memory");
    a->refcount = 1;
    a->length = a->capacity = n;
    if (n) memcpy(a->data, values, n * sizeof(int64_t));
    return a;
}
S_Array_i64 *s_array_i64_retain(S_Array_i64 *a) {
    if (a) a->refcount++;
    return a;
}
void s_array_i64_release(S_Array_i64 *a) {
    if (a && --a->refcount == 0) free(a);
}
S_Optional_i64 s_array_i64_get(const S_Array_i64 *a, int64_t i) {
    if (!a || i < 0 || (uint64_t)i >= a->length) return (S_Optional_i64){false, 0};
    return (S_Optional_i64){true, a->data[i]};
}
int64_t s_array_i64_get_checked(const S_Array_i64 *a, int64_t i) {
    S_Optional_i64 result = s_array_i64_get(a, i);
    if (!result.has_value) s_panic("index out of bounds");
    return result.value;
}
void s_array_i64_set(S_Array_i64 **a, int64_t i, int64_t value) {
    if (!*a || i < 0 || (uint64_t)i >= (*a)->length) s_panic("index out of bounds");
    if ((*a)->refcount != 1) {
        S_Array_i64 *replacement = s_array_i64_make((*a)->length, (*a)->data);
        s_array_i64_release(*a);
        *a = replacement;
    }
    (*a)->data[i] = value;
}
S_String *s_string_from_utf8(const char *str) {
    size_t n = strlen(str);
    S_String *s = malloc(sizeof(*s) + n + 1);
    if (!s) s_panic("out of memory");
    s->refcount = 1;
    s->length = n;
    memcpy(s->data, str, n + 1);
    return s;
}
S_String *s_string_retain(S_String *s) {
    if (s) s->refcount++;
    return s;
}
void s_string_release(S_String *s) {
    if (s && --s->refcount == 0) free(s);
}
S_String *s_string_concat(const S_String *a, const S_String *b) {
    if (a->length > SIZE_MAX - b->length - sizeof(S_String) - 1) s_panic("string too large");
    size_t n = a->length + b->length;
    S_String *s = malloc(sizeof(*s) + n + 1);
    if (!s) s_panic("out of memory");
    s->refcount = 1;
    s->length = n;
    memcpy(s->data, a->data, a->length);
    memcpy(s->data + a->length, b->data, b->length);
    s->data[n] = '\0';
    return s;
}
void s_print_string(const S_String *s) { fwrite(s->data, 1, s->length, stdout); putchar('\n'); }
void s_print_i64(int64_t x) { printf("%" PRId64 "\n", x); }
void s_print_f64(double x) { printf("%.15g\n", x); }
void s_print_bool(bool x) { puts(x ? "true" : "false"); }

/* Small, deterministic hash map used by the golden C examples. */
typedef struct { bool used; int64_t key; int64_t value; } S_DictEntry;
struct S_Dict_i64_i64 {
    size_t refcount;
    size_t count;
    size_t capacity;
    S_DictEntry *entries;
};
static uint64_t s_hash_i64(int64_t key) {
    uint64_t x = (uint64_t)key;
    x ^= x >> 30; x *= UINT64_C(0xbf58476d1ce4e5b9);
    x ^= x >> 27; x *= UINT64_C(0x94d049bb133111eb);
    return x ^ (x >> 31);
}
S_Dict_i64_i64 *s_dict_i64_i64_new(void) {
    S_Dict_i64_i64 *m = calloc(1, sizeof(*m));
    if (!m) s_panic("out of memory");
    m->capacity = 8;
    m->refcount = 1;
    m->entries = calloc(m->capacity, sizeof(*m->entries));
    if (!m->entries) s_panic("out of memory");
    return m;
}
S_Dict_i64_i64 *s_dict_i64_i64_retain(S_Dict_i64_i64 *m) { if (m) m->refcount++; return m; }
void s_dict_i64_i64_release(S_Dict_i64_i64 *m) {
    if (m && --m->refcount == 0) { free(m->entries); free(m); }
}
static void s_map_put_raw(S_Dict_i64_i64 *m, int64_t key, int64_t value) {
    size_t index = (size_t)s_hash_i64(key) & (m->capacity - 1);
    while (m->entries[index].used && m->entries[index].key != key)
        index = (index + 1) & (m->capacity - 1);
    if (!m->entries[index].used) m->count++;
    m->entries[index].used = true;
    m->entries[index].key = key;
    m->entries[index].value = value;
}
static void s_map_grow(S_Dict_i64_i64 *m) {
    size_t old_capacity = m->capacity;
    if (old_capacity > SIZE_MAX / 2) s_panic("map too large");
    S_DictEntry *old_entries = m->entries;
    m->capacity *= 2;
    m->count = 0;
    m->entries = calloc(m->capacity, sizeof(*m->entries));
    if (!m->entries) s_panic("out of memory");
    for (size_t i = 0; i < old_capacity; i++) {
        S_DictEntry e = old_entries[i];
        if (e.used) s_map_put_raw(m, e.key, e.value);
    }
    free(old_entries);
}
S_Dict_i64_i64 *s_dict_i64_i64_insert(S_Dict_i64_i64 *m, int64_t key, int64_t value) {
    if (!m) s_panic("invalid map");
    S_Dict_i64_i64 *result = m;
    if (m->refcount != 1) {
        result = s_dict_i64_i64_new();
        for (size_t i = 0; i < m->capacity; i++) {
            if (m->entries[i].used) {
                if ((result->count + 1) * 4 >= result->capacity * 3) s_map_grow(result);
                s_map_put_raw(result, m->entries[i].key, m->entries[i].value);
            }
        }
        s_dict_i64_i64_release(m); /* consumes the variable's ownership */
    }
    if ((result->count + 1) * 4 >= result->capacity * 3) s_map_grow(result);
    s_map_put_raw(result, key, value);
    return result;
}
S_Optional_i64 s_dict_i64_i64_get(const S_Dict_i64_i64 *m, int64_t key) {
    if (!m) return (S_Optional_i64){false, 0};
    size_t index = (size_t)s_hash_i64(key) & (m->capacity - 1);
    while (m->entries[index].used) {
        if (m->entries[index].key == key)
            return (S_Optional_i64){true, m->entries[index].value};
        index = (index + 1) & (m->capacity - 1);
    }
    return (S_Optional_i64){false, 0};
}

bool s_write_text(const S_String *path, const S_String *text) {
    FILE *f = fopen(path->data, "wb");
    if (!f) return false;
    size_t n = fwrite(text->data, 1, text->length, f);
    int closed = fclose(f);
    return n == text->length && closed == 0;
}
S_Optional_string s_read_text(const S_String *path) {
    FILE *f = fopen(path->data, "rb");
    if (!f) return (S_Optional_string){false, NULL};
    if (fseek(f, 0, SEEK_END)) { fclose(f); return (S_Optional_string){false, NULL}; }
    long length = ftell(f);
    if (length < 0 || fseek(f, 0, SEEK_SET)) { fclose(f); return (S_Optional_string){false, NULL}; }
    size_t n = (size_t)length;
    if (n > SIZE_MAX - sizeof(S_String) - 1) s_panic("file too large");
    S_String *s = malloc(sizeof(*s) + n + 1);
    if (!s) s_panic("out of memory");
    s->refcount = 1; s->length = n;
    size_t r = fread(s->data, 1, n, f);
    int closed = fclose(f);
    if (r != n || closed) { free(s); return (S_Optional_string){false, NULL}; }
    s->data[n] = '\0';
    return (S_Optional_string){true, s};
}
