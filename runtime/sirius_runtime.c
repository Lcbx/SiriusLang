#if !defined(_WIN32)
#define _POSIX_C_SOURCE 200809L
#endif
#include "sirius_runtime.h"
#include <inttypes.h>
#include <limits.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(_WIN32)
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif
#if defined(_WIN32)
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

#ifdef SIRIUS_ARC_AUDIT
/* Debug-only shadow credits for all ARC-managed native objects. Each owned
   child edge counts via the real retain/release hooks, not via pointer identity.
   This also detects objects leaked from unreachable cycles or dropped frames. */
typedef struct S_ArcAuditNode {
    const void *object;
    size_t credits, incoming;
    S_ArcAuditScan scan;
    struct S_ArcAuditNode *next;
} S_ArcAuditNode;
static S_ArcAuditNode *s_arc_audit_objects;
static S_ArcAuditFrame *s_arc_audit_frames;
typedef struct S_ArcAuditGlobalRoot {
    const void *slot;
    struct S_ArcAuditGlobalRoot *next;
} S_ArcAuditGlobalRoot;
static S_ArcAuditGlobalRoot *s_arc_audit_globals;
static bool s_arc_audit_started;
static bool s_arc_audit_terminating;
static void s_arc_audit_fail(const char *problem) {
    fprintf(stderr, "Sirius ARC audit: %s\n", problem);
    fflush(stderr);
    _Exit(86);
}
static S_ArcAuditNode **s_arc_audit_find(const void *object) {
    S_ArcAuditNode **slot = &s_arc_audit_objects;
    while (*slot && (*slot)->object != object) slot = &(*slot)->next;
    return slot;
}
static void s_arc_audit_report(void) {
    /* A fatal process exit has no live caller to return ownership to. */
    if (s_arc_audit_terminating) return;
    if (s_arc_audit_frames) s_arc_audit_fail("ARC frames left on normal exit");
    size_t objects = 0, credits = 0;
    for (S_ArcAuditNode *node = s_arc_audit_objects; node; node = node->next) {
        objects++;
        credits += node->credits;
    }
    if (objects) {
        fprintf(stderr, "Sirius ARC audit: %zu live objects, %zu outstanding credits\n", objects, credits);
        fflush(stderr);
        _Exit(86);
    }
}
void s_arc_audit_begin(void) {
    if (s_arc_audit_started) return;
    if (atexit(s_arc_audit_report)) s_arc_audit_fail("cannot register final report");
    s_arc_audit_started = true;
}
void s_arc_audit_new(const void *object, size_t actual) {
    s_arc_audit_begin();
    if (!object || actual != 1 || *s_arc_audit_find(object)) s_arc_audit_fail("invalid allocation credit");
    S_ArcAuditNode *node = malloc(sizeof(*node));
    if (!node) s_arc_audit_fail("out of memory");
    *node = (S_ArcAuditNode){object, 1, 0, NULL, s_arc_audit_objects};
    s_arc_audit_objects = node;
}
void s_arc_audit_retain(const void *object, size_t actual) {
    if (!object) return;
    S_ArcAuditNode *node = *s_arc_audit_find(object);
    if (!node || node->credits == SIZE_MAX || actual != node->credits + 1) {
        s_arc_audit_fail("retain credit mismatch");
    }
    node->credits++;
}
void s_arc_audit_release(const void *object, size_t actual) {
    if (!object) return;
    S_ArcAuditNode **slot = s_arc_audit_find(object);
    S_ArcAuditNode *node = *slot;
    if (!node || !node->credits || actual != node->credits - 1)
        s_arc_audit_fail("release credit mismatch");
    if (actual) node->credits--;
    else { *slot = node->next; free(node); }
}
void s_arc_audit_watch(const void *object, S_ArcAuditScan scan) {
    S_ArcAuditNode *node = *s_arc_audit_find(object);
    if (!node || !scan) s_arc_audit_fail("invalid edge scanner");
    node->scan = scan;
}
static void s_arc_audit_visit(const void *child, void *context) {
    (void)context;
    if (!child) return;
    S_ArcAuditNode *node = *s_arc_audit_find(child);
    if (!node) s_arc_audit_fail("untracked heap child edge");
    if (node->incoming == SIZE_MAX) s_arc_audit_fail("heap child edge overflow");
    node->incoming++;
}
void s_arc_audit_check_edges(void) {
    /* Scan a stable heap state; no callbacks may retain/release/allocate. */
    for (S_ArcAuditNode *node = s_arc_audit_objects; node; node = node->next)
        node->incoming = 0;
    for (S_ArcAuditNode *node = s_arc_audit_objects; node; node = node->next)
        if (node->scan) node->scan(node->object, s_arc_audit_visit, NULL);
    for (S_ArcAuditNode *node = s_arc_audit_objects; node; node = node->next)
        if (node->credits < node->incoming) s_arc_audit_fail("heap child credits missing");
}
void s_arc_audit_check_roots(const void *const *roots, size_t count) {
    if (!roots && count) s_arc_audit_fail("invalid root list");
    s_arc_audit_check_edges();
    for (size_t i = 0; i < count; ++i) s_arc_audit_visit(roots[i], NULL);
    for (S_ArcAuditNode *node = s_arc_audit_objects; node; node = node->next)
        if (node->credits != node->incoming) s_arc_audit_fail("heap root/child credit mismatch");
}
void s_arc_audit_frame_enter(S_ArcAuditFrame *frame, const void *const *slots, size_t count) {
    if (!frame || (!slots && count)) s_arc_audit_fail("invalid ARC frame");
    frame->previous = s_arc_audit_frames;
    frame->slots = slots;
    frame->count = count;
    s_arc_audit_frames = frame;
}
void s_arc_audit_frame_leave(S_ArcAuditFrame *frame) {
    if (s_arc_audit_frames != frame) s_arc_audit_fail("ARC frame stack mismatch");
    s_arc_audit_frames = frame->previous;
}
void s_arc_audit_global_root(const void *slot) {
    if (!slot) s_arc_audit_fail("invalid global root");
    for (S_ArcAuditGlobalRoot *root = s_arc_audit_globals; root; root = root->next)
        if (root->slot == slot) return;
    S_ArcAuditGlobalRoot *root = malloc(sizeof(*root));
    if (!root) s_arc_audit_fail("out of memory");
    *root = (S_ArcAuditGlobalRoot){slot, s_arc_audit_globals};
    s_arc_audit_globals = root;
}
void s_arc_audit_check_frames(void) {
    s_arc_audit_check_edges();
    for (const S_ArcAuditFrame *frame = s_arc_audit_frames; frame; frame = frame->previous) {
        for (size_t i = 0; i < frame->count; ++i) {
            /* Slots may contain S_String* or S_MirRecord*. Read their common
               object-pointer representation without aliasing through void**. */
            const void *root = NULL;
            memcpy(&root, frame->slots[i], sizeof(root));
            s_arc_audit_visit(root, NULL);
        }
    }
    for (const S_ArcAuditGlobalRoot *root = s_arc_audit_globals; root; root = root->next) {
        const void *object = NULL;
        memcpy(&object, root->slot, sizeof(object));
        s_arc_audit_visit(object, NULL);
    }
    for (S_ArcAuditNode *node = s_arc_audit_objects; node; node = node->next)
        if (node->credits != node->incoming) s_arc_audit_fail("heap root/child credit mismatch");
}
void s_arc_audit_relocate(uintptr_t previous, const void *object) {
    S_ArcAuditNode *node = NULL;
    for (S_ArcAuditNode *entry = s_arc_audit_objects; entry; entry = entry->next)
        if ((uintptr_t)entry->object == previous) { node = entry; break; }
    if (!node || !object) s_arc_audit_fail("invalid relocated object");
    if ((uintptr_t)object != previous && *s_arc_audit_find(object))
        s_arc_audit_fail("duplicate relocated object");
    node->object = object;
}
#endif

_Noreturn void s_panic(const char *reason) {
#ifdef SIRIUS_ARC_AUDIT
    /* Preserve the fatal diagnostic; check credit transitions up to the
       panic, but do not mistake un-unwound process frames for leaks. */
    s_arc_audit_terminating = true;
#endif
    fprintf(stderr, "Sirius panic: %s\n", reason);
    exit(1);
}

S_Array_i64 *s_array_i64_make(size_t n, const int64_t *values) {
    if (n > (SIZE_MAX - sizeof(S_Array_i64)) / sizeof(int64_t)) s_panic("array too large");
    S_Array_i64 *a = malloc(sizeof(*a) + n * sizeof(int64_t));
    if (!a) s_panic("out of memory");
    a->refcount = 1;
    s_arc_audit_new(a, a->refcount);
    a->length = a->capacity = n;
    if (n && values) memcpy(a->data, values, n * sizeof(int64_t));
    return a;
}
S_Array_i64 *s_array_i64_retain(S_Array_i64 *a) {
    if (a) { a->refcount++; s_arc_audit_retain(a, a->refcount); }
    return a;
}
void s_array_i64_release(S_Array_i64 *a) {
    if (a) { --a->refcount; s_arc_audit_release(a, a->refcount); if (!a->refcount) free(a); }
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
S_Array_i64 *s_array_i64_append_take(S_Array_i64 *a, int64_t value) {
    if (!a || a->length == SIZE_MAX) s_panic("array too large");
    size_t n = a->length + 1;
    if (n > (SIZE_MAX - sizeof(*a)) / sizeof(int64_t)) s_panic("array too large");
    if (a->refcount != 1) {
        S_Array_i64 *copy = s_array_i64_make(n, NULL);
        memcpy(copy->data, a->data, a->length * sizeof(int64_t));
        copy->length = a->length;
        s_array_i64_release(a);
        a = copy;
    } else if (n > a->capacity) {
        size_t capacity = a->capacity ? a->capacity : 1;
        while (capacity < n && capacity <= (SIZE_MAX - sizeof(*a)) / sizeof(int64_t) / 2)
            capacity *= 2;
        if (capacity < n) capacity = n;
        uintptr_t previous = (uintptr_t)a;
        a = realloc(a, sizeof(*a) + capacity * sizeof(int64_t));
        if (!a) s_panic("out of memory");
        s_arc_audit_relocate(previous, a);
        a->capacity = capacity;
    }
    a->data[a->length++] = value;
    return a;
}
int64_t s_array_i64_length_take(S_Array_i64 *a) {
    int64_t result = (int64_t)a->length;
    s_array_i64_release(a);
    return result;
}
int64_t s_array_i64_get_checked_take(S_Array_i64 *a, int64_t index) {
    int64_t value = s_array_i64_get_checked(a, index);
    s_array_i64_release(a);
    return value;
}
S_String *s_string_from_utf8(const char *str) {
    return s_string_from_bytes(str, strlen(str));
}
S_String *s_string_retain(S_String *s) {
    if (s) { s->refcount++; s_arc_audit_retain(s, s->refcount); }
    return s;
}
void s_string_release(S_String *s) {
    if (s) { --s->refcount; s_arc_audit_release(s, s->refcount); if (!s->refcount) free(s); }
}
S_String *s_string_concat(const S_String *a, const S_String *b) {
    if (a->length > SIZE_MAX - b->length - sizeof(S_String) - 1) s_panic("string too large");
    size_t n = a->length + b->length;
    S_String *s = malloc(sizeof(*s) + n + 1);
    if (!s) s_panic("out of memory");
    s->refcount = 1;
    s_arc_audit_new(s, s->refcount);
    s->length = n;
    memcpy(s->data, a->data, a->length);
    memcpy(s->data + a->length, b->data, b->length);
    s->data[n] = '\0';
    return s;
}
S_String *s_string_concat_take(S_String *a, S_String *b) {
    S_String *result = s_string_concat(a, b);
    s_string_release(a);
    s_string_release(b);
    return result;
}
int s_string_compare_take(S_String *a, S_String *b) {
    int result = s_string_compare(a, b);
    s_string_release(a);
    s_string_release(b);
    return result;
}
int64_t s_string_length_take(S_String *s) {
    int64_t result = (int64_t)s_string_length(s);
    s_string_release(s);
    return result;
}
void s_print_string_take(S_String *s) {
    s_print_string(s);
    s_string_release(s);
}
void s_print_string(const S_String *s) { fwrite(s->data, 1, s->length, stdout); putchar('\n'); }
void s_print_i64(int64_t x) { printf("%" PRId64 "\n", x); }
void s_print_f64(double x) { printf("%.15g\n", x); }
void s_print_bool(bool x) { puts(x ? "true" : "false"); }
void s_eprint_string(const S_String *s) { fwrite(s->data, 1, s->length, stderr); fputc('\n', stderr); }
void s_eprint_string_take(S_String *s) { s_eprint_string(s); s_string_release(s); }
void s_eprint_i64(int64_t x) { fprintf(stderr, "%" PRId64 "\n", x); }
void s_eprint_f64(double x) { fprintf(stderr, "%.15g\n", x); }
void s_eprint_bool(bool x) { fputs(x ? "true\n" : "false\n", stderr); }

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
    s_arc_audit_new(m, m->refcount);
    m->entries = calloc(m->capacity, sizeof(*m->entries));
    if (!m->entries) s_panic("out of memory");
    return m;
}
S_Dict_i64_i64 *s_dict_i64_i64_retain(S_Dict_i64_i64 *m) { if (m) { m->refcount++; s_arc_audit_retain(m, m->refcount); } return m; }
void s_dict_i64_i64_release(S_Dict_i64_i64 *m) {
    if (m) { --m->refcount; s_arc_audit_release(m, m->refcount); if (!m->refcount) { free(m->entries); free(m); } }
}
static size_t s_map_slot(const S_Dict_i64_i64 *m, int64_t key) {
    size_t index = (size_t)s_hash_i64(key) & (m->capacity - 1);
    while (m->entries[index].used && m->entries[index].key != key)
        index = (index + 1) & (m->capacity - 1);
    return index;
}
static void s_map_put_raw(S_Dict_i64_i64 *m, int64_t key, int64_t value) {
    size_t index = s_map_slot(m, key);
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
        /* Entries contain only scalars: preserve their table layout on CoW. */
        result = malloc(sizeof(*result));
        if (!result) s_panic("out of memory");
        *result = *m;
        result->refcount = 1;
        s_arc_audit_new(result, result->refcount);
        if (m->capacity > SIZE_MAX / sizeof(*m->entries)) s_panic("map too large");
        result->entries = malloc(m->capacity * sizeof(*m->entries));
        if (!result->entries) s_panic("out of memory");
        memcpy(result->entries, m->entries, m->capacity * sizeof(*m->entries));
        s_dict_i64_i64_release(m); /* consumes the variable's ownership */
    }
    size_t slot = s_map_slot(result, key);
    if (!result->entries[slot].used && result->count + 1 >= result->capacity - result->capacity / 4) {
        s_map_grow(result);
        slot = s_map_slot(result, key);
    }
    if (!result->entries[slot].used) result->count++;
    result->entries[slot] = (S_DictEntry){true, key, value};
    return result;
}
S_Optional_i64 s_dict_i64_i64_get(const S_Dict_i64_i64 *m, int64_t key) {
    if (!m) return (S_Optional_i64){false, 0};
    size_t index = s_map_slot(m, key);
    if (m->entries[index].used) return (S_Optional_i64){true, m->entries[index].value};
    return (S_Optional_i64){false, 0};
}

/* Stable portable categories: 0=ok, 1=not found, 2=permission denied,
   3=not a directory, 4=is a directory, 5=invalid path, 6=other I/O. */
#ifdef _MSC_VER
#define S_FS_TLS __declspec(thread)
#else
#define S_FS_TLS _Thread_local
#endif
static S_FS_TLS int64_t s_fs_last_kind;
static S_FS_TLS int64_t s_fs_last_code;
int64_t s_filesystem_error_kind(void) { return s_fs_last_kind; }
int64_t s_filesystem_error_code(void) { return s_fs_last_code; }
static void s_fs_success(void) { s_fs_last_kind = 0; s_fs_last_code = 0; }
static void s_fs_errno(int code) {
    if (!code) code = EIO;
    s_fs_last_code = code;
    switch (code) {
        case ENOENT: s_fs_last_kind = 1; break;
        case EACCES: case EPERM: s_fs_last_kind = 2; break;
#ifdef ENOTDIR
        case ENOTDIR: s_fs_last_kind = 3; break;
#endif
#ifdef EISDIR
        case EISDIR: s_fs_last_kind = 4; break;
#endif
        case EINVAL: s_fs_last_kind = 5; break;
#ifdef ENAMETOOLONG
        case ENAMETOOLONG: s_fs_last_kind = 5; break;
#endif
        default: s_fs_last_kind = 6; break;
    }
}
#ifdef _WIN32
static void s_fs_win32(DWORD code) {
    s_fs_last_code = code ? (int64_t)code : ERROR_GEN_FAILURE;
    switch (code) {
        case ERROR_FILE_NOT_FOUND: case ERROR_PATH_NOT_FOUND: s_fs_last_kind = 1; break;
        case ERROR_ACCESS_DENIED: case ERROR_SHARING_VIOLATION: s_fs_last_kind = 2; break;
        case ERROR_DIRECTORY: s_fs_last_kind = 3; break;
        case ERROR_INVALID_NAME: case ERROR_BAD_PATHNAME: s_fs_last_kind = 5; break;
        default: s_fs_last_kind = 6; break;
    }
}
#endif

#ifdef _WIN32
static wchar_t *s_fs_wide(const S_String *path);
#endif
/* The Windows CRT uses wide paths for UTF-8 Sirius strings. MSVC warns about
   fopen/_wfopen; the secure variant is MSVC-specific, not portable C. */
static FILE *s_fs_open(const S_String *path, bool writing) {
#ifdef _WIN32
    wchar_t *wide = s_fs_wide(path);
    if (!wide) return NULL;
    FILE *file = NULL;
#ifdef _MSC_VER
    int status = _wfopen_s(&file, wide, writing ? L"wb" : L"rb");
    if (status) { errno = status; file = NULL; }
#else
    file = _wfopen(wide, writing ? L"wb" : L"rb");
#endif
    free(wide);
    return file;
#else
    if (memchr(path->data, 0, path->length)) { errno = EINVAL; return NULL; }
    return fopen(path->data, writing ? "wb" : "rb");
#endif
}

bool s_write_text(const S_String *path, const S_String *text) {
    FILE *f = s_fs_open(path, true);
    if (!f) { s_fs_errno(errno); return false; }
    errno = 0;
    size_t n = fwrite(text->data, 1, text->length, f);
    int write_error = n == text->length ? 0 : (errno ? errno : EIO);
    int closed = fclose(f);
    if (write_error || closed) { s_fs_errno(write_error ? write_error : errno); return false; }
    s_fs_success();
    return true;
}
S_Optional_string s_read_text(const S_String *path) {
    FILE *f = s_fs_open(path, false);
    if (!f) { s_fs_errno(errno); return (S_Optional_string){false, NULL}; }
    if (fseek(f, 0, SEEK_END)) {
        int code = errno; fclose(f); s_fs_errno(code);
        return (S_Optional_string){false, NULL};
    }
    long length = ftell(f);
    if (length < 0 || fseek(f, 0, SEEK_SET)) {
        int code = errno; fclose(f); s_fs_errno(code);
        return (S_Optional_string){false, NULL};
    }
    size_t n = (size_t)length;
    if (n > SIZE_MAX - sizeof(S_String) - 1) s_panic("file too large");
    S_String *s = malloc(sizeof(*s) + n + 1);
    if (!s) s_panic("out of memory");
    s->refcount = 1; s_arc_audit_new(s, s->refcount); s->length = n;
    errno = 0;
    size_t r = fread(s->data, 1, n, f);
    int read_error = r == n ? 0 : (errno ? errno : EIO);
    int closed = fclose(f);
    if (read_error || closed) {
        s_string_release(s);
        s_fs_errno(read_error ? read_error : errno);
        return (S_Optional_string){false, NULL};
    }
    s->data[n] = '\0';
    s_fs_success();
    return (S_Optional_string){true, s};
}

/* Generic homogeneous collection storage. Element buffers are allocated by
   malloc (adequately aligned for any C object), not as packed byte fields. */
struct S_Array {
    size_t refcount, length, capacity, size;
    S_CopyElem copy;
    S_DropElem drop;
    void *data;
};
static void *s_alloc(size_t count, size_t size) {
    if (size && count > SIZE_MAX / size) s_panic("allocation overflow");
    void *ptr = calloc(count ? count : 1, size ? size : 1);
    if (!ptr) s_panic("out of memory");
    return ptr;
}
static void s_elem_copy(S_CopyElem cb, void *dst, const void *src, size_t size) {
    if (cb) cb(dst, src);
    else memcpy(dst, src, size);
}
/* Most array elements fit on the stack. Preserve a temporary copy before
   reallocating, replacing, or dropping storage that may alias the RHS. */
typedef union { max_align_t align; unsigned char bytes[128]; } S_ElementTemp;
static void *s_elem_temp(S_ElementTemp *temp, size_t size) {
    return size <= sizeof(temp->bytes) ? temp->bytes : s_alloc(1, size);
}
static void s_elem_temp_free(S_ElementTemp *temp, void *ptr) {
    if (ptr != temp->bytes) free(ptr);
}
S_Array *s_array_new(size_t size, S_CopyElem copy, S_DropElem drop) {
    if (!size) s_panic("zero-sized array element");
    S_Array *a = s_alloc(1, sizeof(*a));
    a->refcount = 1; s_arc_audit_new(a, a->refcount); a->size = size; a->copy = copy; a->drop = drop;
    return a;
}
S_Array *s_array_make(size_t n, const void *values, size_t size, S_CopyElem copy, S_DropElem drop) {
    S_Array *a = s_array_new(size, copy, drop);
    a->capacity = a->length = n;
    a->data = s_alloc(n, size);
    for (size_t i=0; i<n; i++)
        s_elem_copy(copy, (char*)a->data+i*size, (const char*)values+i*size, size);
    return a;
}
S_Array *s_array_retain(S_Array *a) { if (a) { a->refcount++; s_arc_audit_retain(a, a->refcount); } return a; }
void s_array_release(S_Array *a) {
    if (!a) return;
    --a->refcount; s_arc_audit_release(a, a->refcount);
    if (a->refcount) return;
    if (a->drop) for (size_t i=0; i<a->length; i++) a->drop((char*)a->data+i*a->size);
    free(a->data); free(a);
}
size_t s_array_length(const S_Array *a) { return a ? a->length : 0; }
const void *s_array_at(const S_Array *a, int64_t i) {
    if (!a || i < 0 || (uint64_t)i >= a->length) return NULL;
    return (const char*)a->data+(size_t)i*a->size;
}
static void s_array_unique(S_Array **owned) {
    S_Array *a = *owned;
    if (!a) s_panic("null array");
    if (a->refcount == 1) return;
    S_Array *new_array = s_array_make(a->length, a->data, a->size, a->copy, a->drop);
    s_array_release(a); *owned = new_array;
}
static void s_array_reserve(S_Array *a, size_t need) {
    if (need <= a->capacity) return;
    size_t capacity = a->capacity ? a->capacity : 4;
    while (capacity < need) {
        if (capacity > SIZE_MAX/2) s_panic("array too large");
        capacity *= 2;
    }
    if (capacity > SIZE_MAX / a->size) s_panic("array too large");
    void *data = realloc(a->data, capacity * a->size);
    if (!data) s_panic("out of memory");
    a->data = data; a->capacity = capacity;
}
S_Array *s_array_append(S_Array *owned, const void *item) {
    s_array_unique(&owned);
    if (owned->length == SIZE_MAX) s_panic("array too large");
    /* Preserve an item borrowed from this same array across reallocation. */
    S_ElementTemp temp = {0};
    void *saved = s_elem_temp(&temp, owned->size);
    s_elem_copy(owned->copy, saved, item, owned->size);
    s_array_reserve(owned, owned->length+1);
    memcpy((char*)owned->data+owned->length*owned->size, saved, owned->size);
    s_elem_temp_free(&temp, saved); /* ownership moved into the array */
    owned->length++;
    return owned;
}
void s_array_set(S_Array **owned, int64_t idx, const void *item) {
    if (!s_array_at(*owned, idx)) s_panic("index out of bounds");
    /* The RHS may point into the array itself. Copy it before CoW or dropping. */
    S_Array *a = *owned;
    S_ElementTemp temp = {0};
    void *saved = s_elem_temp(&temp, a->size);
    s_elem_copy(a->copy, saved, item, a->size);
    s_array_unique(owned);
    a = *owned;
    void *place = (char*)a->data+(size_t)idx*a->size;
    if (a->drop) a->drop(place);
    memcpy(place, saved, a->size); /* transfer saved's ownership */
    s_elem_temp_free(&temp, saved);
}

/* Optional counters for profiling dictionaries in compiled Sirius programs.
   Enabled only in dedicated profiling builds; no production overhead. */
#ifdef S_DICT_PROFILE
static struct {
    size_t creations, empty_releases, inserts, overwrites, gets, misses, clones, cloned_entries;
    size_t grows, slot_calls, occupied_probes, equality_checks;
    size_t size_le5, size_le11, size_gt11;
} s_dict_profile;
static void s_dict_profile_report(void) {
    fprintf(stderr, "S_DICT_PROFILE new=%zu empty_release=%zu insert=%zu overwrite=%zu get=%zu miss=%zu "
        "clone=%zu cloned_entries=%zu grow=%zu slots=%zu probes=%zu equal=%zu "
        "size_le5=%zu size_le11=%zu size_gt11=%zu\n",
        s_dict_profile.creations, s_dict_profile.empty_releases, s_dict_profile.inserts, s_dict_profile.overwrites,
        s_dict_profile.gets, s_dict_profile.misses, s_dict_profile.clones,
        s_dict_profile.cloned_entries, s_dict_profile.grows, s_dict_profile.slot_calls,
        s_dict_profile.occupied_probes, s_dict_profile.equality_checks,
        s_dict_profile.size_le5, s_dict_profile.size_le11, s_dict_profile.size_gt11);
}
#define DICT_PROFILE(statement) do { statement; } while (0)
#else
#define DICT_PROFILE(statement) ((void)0)
#endif

/* Open-addressed hash map. Each occupied key/value allocation is owned;
   resizing transfers pointers and CoW clones retain every managed element. */
typedef struct { bool used; uint64_t hash; void *key; void *value; } S_GEntry;
struct S_Dict {
    size_t refcount, length, capacity, ksize, vsize, value_offset;
    S_CopyElem kcopy, vcopy;
    S_DropElem kdrop, vdrop;
    S_HashKey hash;
    S_EqualKey eq;
    S_GEntry *entries;
#ifdef SIRIUS_ARC_AUDIT
    bool audit_key_managed, audit_value_managed;
#endif
};
#ifdef SIRIUS_ARC_AUDIT
static void s_dict_arc_audit_scan(const void *object, S_ArcAuditVisit visit, void *context) {
    const S_Dict *dict = object;
    for (size_t i = 0; i < dict->capacity; ++i) {
        const S_GEntry *entry = &dict->entries[i];
        if (!entry->used) continue;
        if (dict->audit_key_managed) visit(*(void *const *)entry->key, context);
        if (dict->audit_value_managed) visit(*(void *const *)entry->value, context);
    }
}
void s_dict_arc_audit_fields(S_Dict *dict, bool key_managed, bool value_managed) {
    if (!dict) s_arc_audit_fail("invalid dictionary edge scanner");
    dict->audit_key_managed = key_managed;
    dict->audit_value_managed = value_managed;
    s_arc_audit_watch(dict, s_dict_arc_audit_scan);
}
#endif
static void s_dict_rehash(S_Dict *d, size_t cap) {
    S_GEntry *old = d->entries;
    size_t previous = d->capacity;
    DICT_PROFILE(if (previous) s_dict_profile.grows++);
    d->entries = s_alloc(cap, sizeof(S_GEntry));
    d->capacity = cap;
    for (size_t i=0; i<previous; i++) if (old[i].used) {
        size_t slot = (size_t)old[i].hash & (cap-1);
        while (d->entries[slot].used) slot = (slot+1)&(cap-1);
        d->entries[slot] = old[i];
    }
    free(old);
}
S_Dict *s_dict_new(size_t ksize, size_t vsize, S_CopyElem kcopy, S_DropElem kdrop,
                   S_CopyElem vcopy, S_DropElem vdrop, S_HashKey hash, S_EqualKey eq) {
    if (!ksize || !vsize || !hash || !eq) s_panic("invalid dictionary type");
#ifdef S_DICT_PROFILE
    if (!s_dict_profile.creations) atexit(s_dict_profile_report);
#endif
    DICT_PROFILE(s_dict_profile.creations++);
    size_t alignment = _Alignof(max_align_t);
    if (ksize > SIZE_MAX - (alignment - 1)) s_panic("dictionary element too large");
    size_t offset = ((ksize + alignment - 1) / alignment) * alignment;
    if (vsize > SIZE_MAX - offset) s_panic("dictionary element too large");
    S_Dict *d = s_alloc(1, sizeof(*d));
    d->refcount=1; s_arc_audit_new(d, d->refcount); d->ksize=ksize; d->vsize=vsize; d->value_offset=offset;
    d->kcopy=kcopy; d->kdrop=kdrop; d->vcopy=vcopy; d->vdrop=vdrop;
    d->hash=hash; d->eq=eq;
    s_dict_rehash(d, 8);
    return d;
}
S_Dict *s_dict_retain(S_Dict *d) { if (d) { d->refcount++; s_arc_audit_retain(d, d->refcount); } return d; }
void s_dict_release(S_Dict *d) {
    if (!d) return;
    --d->refcount; s_arc_audit_release(d, d->refcount);
    if (d->refcount) return;
    DICT_PROFILE(if (!d->length) s_dict_profile.empty_releases++);
    DICT_PROFILE(if (d->length <= 5) s_dict_profile.size_le5++;
        else if (d->length <= 11) s_dict_profile.size_le11++;
        else s_dict_profile.size_gt11++);
    for (size_t i=0; i<d->capacity; i++) if (d->entries[i].used) {
        if (d->kdrop) d->kdrop(d->entries[i].key);
        if (d->vdrop) d->vdrop(d->entries[i].value);
        free(d->entries[i].key);
    }
    free(d->entries); free(d);
}
static S_GEntry *s_dict_slot(const S_Dict *d, const void *key, uint64_t hash) {
    size_t slot = (size_t)hash & (d->capacity-1);
    DICT_PROFILE(s_dict_profile.slot_calls++);
    S_GEntry *entries = d->entries;
    while (entries[slot].used) {
        DICT_PROFILE(s_dict_profile.occupied_probes++);
        if (entries[slot].hash == hash) {
            DICT_PROFILE(s_dict_profile.equality_checks++);
            if (d->eq(entries[slot].key, key)) break;
        }
        slot = (slot+1)&(d->capacity-1);
    }
    return &entries[slot];
}
/* One aligned allocation owns both payloads; rehash transfers the entry. */
static void s_dict_entry_copy(S_Dict *d, S_GEntry *entry, const void *key, const void *value) {
    entry->key = s_alloc(1, d->value_offset + d->vsize);
    entry->value = (char *)entry->key + d->value_offset;
    s_elem_copy(d->kcopy, entry->key, key, d->ksize);
    s_elem_copy(d->vcopy, entry->value, value, d->vsize);
}
static S_Dict *s_dict_clone(S_Dict *d) {
    DICT_PROFILE(s_dict_profile.clones++; s_dict_profile.cloned_entries += d->length);
    /* Cloning with the same capacity preserves probe positions. */
    S_Dict *copy = s_alloc(1, sizeof(*copy));
    *copy = *d;
    copy->refcount = 1;
    s_arc_audit_new(copy, copy->refcount);
#ifdef SIRIUS_ARC_AUDIT
    copy->audit_key_managed = d->audit_key_managed;
    copy->audit_value_managed = d->audit_value_managed;
    if (d->audit_key_managed || d->audit_value_managed)
        s_arc_audit_watch(copy, s_dict_arc_audit_scan);
#endif
    copy->entries = s_alloc(d->capacity, sizeof(*copy->entries));
    for (size_t i=0;i<d->capacity;i++) if (d->entries[i].used) {
        S_GEntry *slot = &copy->entries[i];
        s_dict_entry_copy(copy, slot, d->entries[i].key, d->entries[i].value);
        slot->hash = d->entries[i].hash;
        slot->used = true;
    }
    return copy;
}
S_Dict *s_dict_insert(S_Dict *owned, const void *key, const void *value) {
    if (!owned) s_panic("null dictionary");
    DICT_PROFILE(s_dict_profile.inserts++);
    uint64_t hash = owned->hash(key);
    S_GEntry *slot = s_dict_slot(owned, key, hash);
    DICT_PROFILE(if (slot->used) s_dict_profile.overwrites++);
    if (slot->used && owned->refcount == 1) {
        /* Copy first: the value may alias this entry or another managed field.
           Preserve the original equal key and its storage on overwrite. */
        S_ElementTemp temp = {0};
        void *saved = s_elem_temp(&temp, owned->vsize);
        s_elem_copy(owned->vcopy, saved, value, owned->vsize);
        if (owned->vdrop) owned->vdrop(slot->value);
        memcpy(slot->value, saved, owned->vsize);
        s_elem_temp_free(&temp, saved);
        return owned;
    }
    /* New entries and shared snapshots copy their borrowed operands before
       cloning or rehashing can invalidate the original storage. */
    S_GEntry replacement_entry = {0};
    s_dict_entry_copy(owned, &replacement_entry, key, value);
    if (owned->refcount != 1) {
        S_Dict *replacement = s_dict_clone(owned);
        s_dict_release(owned); owned = replacement;
        slot = s_dict_slot(owned, replacement_entry.key, hash);
    }
    /* Overwriting a key does not increase table occupancy. */
    if (!slot->used && owned->length + 1 >= owned->capacity - owned->capacity / 4) {
        if (owned->capacity > SIZE_MAX / 2) s_panic("dictionary too large");
        s_dict_rehash(owned,owned->capacity*2);
        slot=s_dict_slot(owned,replacement_entry.key,hash);
    }
    if (slot->used) {
        if (owned->kdrop) owned->kdrop(slot->key);
        if (owned->vdrop) owned->vdrop(slot->value);
        free(slot->key);
    } else { owned->length++; }
    replacement_entry.used=true; replacement_entry.hash=hash;
    *slot=replacement_entry;
    return owned;
}
const void *s_dict_get(const S_Dict *d, const void *key) {
    DICT_PROFILE(s_dict_profile.gets++);
    if (!d) { DICT_PROFILE(s_dict_profile.misses++); return NULL; }
    S_GEntry *slot=s_dict_slot(d,key,d->hash(key));
    DICT_PROFILE(if (!slot->used) s_dict_profile.misses++);
    return slot->used ? slot->value : NULL;
}
size_t s_dict_length(const S_Dict *d) { return d ? d->length : 0; }
int64_t s_dict_next_slot(const S_Dict *d, int64_t after) {
    if (!d || after < -1 || (after >= 0 && (uint64_t)after >= d->capacity)) s_panic("invalid dictionary cursor");
    for (size_t i = (size_t)(after + 1); i < d->capacity; ++i)
        if (d->entries[i].used) return (int64_t)i;
    return -1;
}
static const S_GEntry *s_dict_checked_slot(const S_Dict *d, int64_t slot) {
    if (!d || slot < 0 || (uint64_t)slot >= d->capacity || !d->entries[slot].used)
        s_panic("invalid dictionary cursor");
    return &d->entries[slot];
}
const void *s_dict_key_slot(const S_Dict *d, int64_t slot) { return s_dict_checked_slot(d, slot)->key; }
const void *s_dict_value_slot(const S_Dict *d, int64_t slot) { return s_dict_checked_slot(d, slot)->value; }
uint64_t s_hash_i64_key(const void *key) { return s_hash_i64(*(const int64_t*)key); }
bool s_eq_i64_key(const void *a,const void *b) { return *(const int64_t*)a == *(const int64_t*)b; }
uint64_t s_hash_string_key(const void *key) {
    const S_String *s=*(S_String *const *)key;
    uint64_t hash=UINT64_C(14695981039346656037);
    for (size_t i=0;i<s->length;i++) { hash ^= (unsigned char)s->data[i]; hash *= UINT64_C(1099511628211); }
    return hash;
}
bool s_eq_string_key(const void *a,const void *b) {
    return s_string_compare(*(S_String *const *)a,*(S_String *const *)b)==0;
}

int s_string_compare(const S_String *a,const S_String *b) {
    size_t min=a->length<b->length?a->length:b->length;
    int cmp=memcmp(a->data,b->data,min);
    if (cmp) return cmp;
    return (a->length>b->length)-(a->length<b->length);
}
size_t s_string_length(const S_String *s) { return s->length; }
S_Optional_i64 s_string_byte_at(const S_String *s,int64_t i) {
    if (i<0 || (uint64_t)i>=s->length) return (S_Optional_i64){false,0};
    return (S_Optional_i64){true,(unsigned char)s->data[i]};
}
S_String *s_string_from_bytes(const char *bytes,size_t n) {
    if (n > SIZE_MAX-sizeof(S_String)-1) s_panic("string too long");
    S_String *s=s_alloc(1,sizeof(*s)+n+1);
    s->refcount=1; s_arc_audit_new(s, s->refcount); s->length=n;
    if (n) memcpy(s->data,bytes,n);
    s->data[n]='\0';
    return s;
}
S_String *s_string_slice(const S_String *s,int64_t from,int64_t to) {
    if (from<0 || to<from || (uint64_t)to>s->length) s_panic("string slice out of bounds");
    return s_string_from_bytes(s->data+from,(size_t)(to-from));
}
S_Optional_i64 s_string_find(const S_String *s,const S_String *needle) {
    if (needle->length>s->length) return (S_Optional_i64){false,0};
    for (size_t i=0;i<=s->length-needle->length;i++)
        if (!memcmp(s->data+i,needle->data,needle->length)) return (S_Optional_i64){true,(int64_t)i};
    return (S_Optional_i64){false,0};
}
bool s_string_starts_with(const S_String *s,const S_String *p) {
    return p->length<=s->length && !memcmp(s->data,p->data,p->length);
}
bool s_string_ends_with(const S_String *s,const S_String *p) {
    return p->length<=s->length && !memcmp(s->data+s->length-p->length,p->data,p->length);
}
S_String *s_string_from_i64(int64_t value) {
    char buf[64];
    int n=snprintf(buf,sizeof(buf),"%" PRId64,value);
    if (n<0 || (size_t)n>=sizeof(buf)) s_panic("integer formatting failed");
    return s_string_from_bytes(buf,(size_t)n);
}
S_String *s_string_join(const S_String *sep,const S_Array *parts) {
    if (parts->size != sizeof(S_String *)) s_panic("join requires string array");
    size_t total=0;
    for (size_t i=0;i<parts->length;i++) {
        S_String *p=*(S_String**)((char*)parts->data+i*parts->size);
        if (p->length>SIZE_MAX-total) s_panic("joined string too long");
        total+=p->length;
        if (i && sep->length>SIZE_MAX-total) s_panic("joined string too long");
        if (i) total+=sep->length;
    }
    S_String *out=s_alloc(1,sizeof(*out)+total+1);
    out->refcount=1;s_arc_audit_new(out, out->refcount);out->length=total;
    size_t offset=0;
    for (size_t i=0;i<parts->length;i++) {
        if (i) { memcpy(out->data+offset,sep->data,sep->length);offset+=sep->length; }
        S_String *p=*(S_String**)((char*)parts->data+i*parts->size);
        memcpy(out->data+offset,p->data,p->length);offset+=p->length;
    }
    out->data[total]='\0'; return out;
}
static S_Array *s_cli_args=NULL;
#ifdef SIRIUS_ARC_AUDIT
static void s_cli_arc_scan(const void *object, S_ArcAuditVisit visit, void *context) {
    const S_Array *array = object;
    for (size_t i = 0; i < array->length; ++i) {
        S_String *value = NULL;
        memcpy(&value, (const char *)array->data + i * array->size, sizeof(value));
        visit(value, context);
    }
}
#endif
static bool s_cli_cleanup_registered=false;
static void s_cli_cleanup(void) { s_array_release(s_cli_args); s_cli_args=NULL; }
static void s_copy_arg(void *dst,const void *src) {
    *(S_String **)dst=s_string_retain(*(S_String * const *)src);
}
static void s_drop_arg(void *item) { s_string_release(*(S_String **)item); }
/* Filesystem discovery is intentionally non-recursive: walk policies belong
   in Sirius. Directory enumeration is deterministic across backends. */
static void s_fs_copy_name(void *dst, const void *src) {
    *(S_String **)dst = s_string_retain(*(S_String *const *)src);
}
static void s_fs_drop_name(void *value) { s_string_release(*(S_String **)value); }
static int s_fs_compare_name(const void *a, const void *b) {
    return s_string_compare(*(S_String *const *)a, *(S_String *const *)b);
}
static S_Array *s_fs_add_name(S_Array *names, const char *bytes, size_t length) {
    if ((length == 1 && bytes[0] == '.') ||
        (length == 2 && bytes[0] == '.' && bytes[1] == '.')) return names;
    S_String *name = s_string_from_bytes(bytes, length);
    names = s_array_append(names, &name);
    s_string_release(name);
    return names;
}
#ifdef _WIN32
static wchar_t *s_fs_wide(const S_String *path) {
    if (memchr(path->data, 0, path->length) || path->length > INT_MAX) { errno = EINVAL; return NULL; }
    int count = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, path->data,
                                    (int)path->length, NULL, 0);
    if (!count) { errno = EINVAL; return NULL; }
    wchar_t *wide = calloc((size_t)count + 1, sizeof(wchar_t));
    if (!wide) s_panic("out of memory");
    if (!MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, path->data,
                              (int)path->length, wide, count)) { free(wide); errno = EINVAL; return NULL; }
    return wide;
}
int64_t s_path_kind(const S_String *path) {
    wchar_t *wide = s_fs_wide(path);
    if (!wide) { s_fs_errno(errno); return 0; }
    DWORD attr = GetFileAttributesW(wide);
    if (attr == INVALID_FILE_ATTRIBUTES) {
        DWORD code = GetLastError();
        free(wide); s_fs_win32(code); return 0;
    }
    free(wide);
    s_fs_success();
    if (attr & FILE_ATTRIBUTE_REPARSE_POINT) return 3;
    return (attr & FILE_ATTRIBUTE_DIRECTORY) ? 2 : 1;
}
bool s_is_dir(const S_String *path) {
    wchar_t *wide = s_fs_wide(path);
    if (!wide) return false;
    DWORD attr = GetFileAttributesW(wide);
    free(wide);
    return attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY) &&
           !(attr & FILE_ATTRIBUTE_REPARSE_POINT);
}
S_Array *s_list_dir(const S_String *path) {
    wchar_t *wide = s_fs_wide(path);
    if (!wide) { s_fs_errno(errno); return NULL; }
    size_t n = wcslen(wide);
    wchar_t *pattern = realloc(wide, (n + 3) * sizeof(wchar_t));
    if (!pattern) { free(wide); s_panic("out of memory"); }
    pattern[n++] = L'\\'; pattern[n++] = L'*'; pattern[n] = 0;
    WIN32_FIND_DATAW item;
    HANDLE handle = FindFirstFileW(pattern, &item);
    free(pattern);
    if (handle == INVALID_HANDLE_VALUE) {
        DWORD code = GetLastError();
        if (code == ERROR_FILE_NOT_FOUND && s_is_dir(path)) {
            s_fs_success();
            return s_array_new(sizeof(S_String *), s_fs_copy_name, s_fs_drop_name);
        }
        s_fs_win32(code);
        return NULL;
    }
    S_Array *names = s_array_new(sizeof(S_String *), s_fs_copy_name, s_fs_drop_name);
    do {
        const wchar_t *name = item.cFileName;
        int bytes = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, name, -1, NULL, 0, NULL, NULL);
        if (bytes <= 0) { DWORD code = GetLastError(); s_array_release(names); FindClose(handle); s_fs_win32(code); return NULL; }
        char *utf8 = malloc((size_t)bytes);
        if (!utf8) s_panic("out of memory");
        if (!WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, name, -1, utf8, bytes, NULL, NULL)) {
            DWORD code = GetLastError(); free(utf8); s_array_release(names); FindClose(handle); s_fs_win32(code); return NULL;
        }
        names = s_fs_add_name(names, utf8, (size_t)bytes - 1);
        free(utf8);
    } while (FindNextFileW(handle, &item));
    DWORD status = GetLastError();
    FindClose(handle);
    if (status != ERROR_NO_MORE_FILES) { s_array_release(names); s_fs_win32(status); return NULL; }
    s_fs_success();
    qsort(names->data, names->length, sizeof(S_String *), s_fs_compare_name);
    return names;
}
#else
int64_t s_path_kind(const S_String *path) {
    if (memchr(path->data, 0, path->length)) { s_fs_errno(EINVAL); return 0; }
    struct stat info;
    if (lstat(path->data, &info)) { s_fs_errno(errno); return 0; }
    s_fs_success();
    if (S_ISREG(info.st_mode)) return 1;
    if (S_ISDIR(info.st_mode)) return 2;
    return 3;
}
bool s_is_dir(const S_String *path) {
    if (memchr(path->data, 0, path->length)) return false;
    struct stat info;
    return lstat(path->data, &info) == 0 && S_ISDIR(info.st_mode);
}
S_Array *s_list_dir(const S_String *path) {
    if (memchr(path->data, 0, path->length)) { s_fs_errno(EINVAL); return NULL; }
    DIR *dir = opendir(path->data);
    if (!dir) { s_fs_errno(errno); return NULL; }
    S_Array *names = s_array_new(sizeof(S_String *), s_fs_copy_name, s_fs_drop_name);
    struct dirent *item;
    errno = 0;
    while ((item = readdir(dir)) != NULL) {
        names = s_fs_add_name(names, item->d_name, strlen(item->d_name));
        errno = 0;
    }
    int read_error = errno;
    int status = closedir(dir);
    if (status || read_error) {
        int code = read_error ? read_error : errno;
        s_array_release(names); s_fs_errno(code); return NULL;
    }
    s_fs_success();
    qsort(names->data, names->length, sizeof(S_String *), s_fs_compare_name);
    return names;
}
#endif

void s_set_arguments(int argc,char **argv) {
    s_arc_audit_begin();
    if (!s_cli_cleanup_registered) {
#ifdef SIRIUS_ARC_AUDIT
        s_arc_audit_global_root(&s_cli_args);
#endif
        if (atexit(s_cli_cleanup)) s_panic("cannot register argument cleanup");
        s_cli_cleanup_registered=true;
    }
    if (s_cli_args) s_array_release(s_cli_args);
    s_cli_args=s_array_new(sizeof(S_String *),s_copy_arg,s_drop_arg);
#ifdef SIRIUS_ARC_AUDIT
    s_arc_audit_watch(s_cli_args, s_cli_arc_scan);
#endif
    for (int i=0;i<argc;i++) {
        S_String *s=s_string_from_utf8(argv[i]);
        s_cli_args=s_array_append(s_cli_args,&s);
        s_string_release(s);
    }
}
S_Array *s_arguments(void) { return s_array_retain(s_cli_args); }

/* The emitter passes owned values. Public primitive functions above borrow. */
S_Optional_i64 s_string_byte_at_take(S_String *s, int64_t index) {
    S_Optional_i64 result = s_string_byte_at(s, index);
    s_string_release(s);
    return result;
}
S_String *s_string_slice_take(S_String *s, int64_t start, int64_t end) {
    S_String *result = s_string_slice(s, start, end);
    s_string_release(s);
    return result;
}
S_Optional_i64 s_string_find_take(S_String *s, S_String *needle) {
    S_Optional_i64 result = s_string_find(s, needle);
    s_string_release(s);
    s_string_release(needle);
    return result;
}
bool s_string_starts_with_take(S_String *s, S_String *prefix) {
    bool result = s_string_starts_with(s, prefix);
    s_string_release(s);
    s_string_release(prefix);
    return result;
}
bool s_string_ends_with_take(S_String *s, S_String *suffix) {
    bool result = s_string_ends_with(s, suffix);
    s_string_release(s);
    s_string_release(suffix);
    return result;
}
S_String *s_string_join_take(S_String *separator, S_Array *parts) {
    S_String *result = s_string_join(separator, parts);
    s_string_release(separator);
    s_array_release(parts);
    return result;
}
S_Optional_string s_read_text_take(S_String *path) {
    S_Optional_string result = s_read_text(path);
    s_string_release(path);
    return result;
}
bool s_write_text_take(S_String *path, S_String *text) {
    bool result = s_write_text(path, text);
    s_string_release(path);
    s_string_release(text);
    return result;
}
