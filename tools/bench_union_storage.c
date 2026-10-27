/*
 * POD-only storage-layout model for Sirius tagged union arrays.
 *
 * This is NOT the Sirius emitter or runtime: it models its two RC allocations
 * per element, a fused RC union, and a by-value, max-width array element.
 * COW copies retain pointed-to unions in the first two models and copy all
 * bytes in the inline model. No managed payload fields or ABI are modeled.
 */
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Same header/field sizes as the generated C runtime on 64-bit targets. */
typedef union { int64_t bits; double real; void *pointer; } Slot;
typedef struct Node {
    int64_t type_id;
    size_t refs, length, capacity;
    Slot fields[];
} Node;

typedef struct {
    unsigned char *data;
    size_t len, cap, stride;
} Buffer;

static uint64_t allocations, reallocations, requested_bytes, peak_bytes, live_bytes;

static void *allocate(size_t bytes) {
    void *p = malloc(bytes ? bytes : 1);
    if (!p) abort();
    ++allocations;
    requested_bytes += bytes;
    live_bytes += bytes;
    if (live_bytes > peak_bytes) peak_bytes = live_bytes;
    return p;
}
static void *resize(void *p, size_t old, size_t next) {
    void *q = realloc(p, next ? next : 1);
    if (!q) abort();
    ++reallocations;
    requested_bytes += next - (next < old ? next : old);
    live_bytes = live_bytes - old + next;
    if (live_bytes > peak_bytes) peak_bytes = live_bytes;
    return q;
}
static void deallocate(void *p, size_t bytes) {
    if (p) { live_bytes -= bytes; free(p); }
}
static Node *new_node(int64_t type_id, size_t slots) {
    Node *n = allocate(sizeof(Node) + slots * sizeof(Slot));
    n->type_id = type_id;
    n->refs = 1;
    n->length = n->capacity = slots;
    memset(n->fields, 0, slots * sizeof(Slot));
    return n;
}
static void release_node(Node *node, int model) {
    if (--node->refs) return;
    if (model == 0 && node->type_id == 2)
        release_node(node->fields[1].pointer, model);
    deallocate(node, sizeof(Node) + node->capacity * sizeof(Slot));
}
static void reserve(Buffer *b, size_t count) {
    if (b->cap >= count) return;
    size_t cap = b->cap ? b->cap : 4;
    while (cap < count) cap *= 2;
    b->data = resize(b->data, b->cap * b->stride, cap * b->stride);
    b->cap = cap;
}
static size_t width_for(size_t i, size_t maximum, size_t period) {
    return maximum && (i % period == 0) ? maximum : maximum ? 1 : 0;
}
static int64_t value_for(size_t i, size_t width) {
    return (int64_t)i + (int64_t)width + 1;
}
static int64_t tag_for(size_t i, size_t period) {
    return i % period == 0;
}
static Node *make_variant(int model, size_t i, size_t maximum, size_t period,
                          int changed) {
    size_t width = width_for(i, maximum, period);
    Node *wrapper;
    if (model == 0) {
        Node *record = new_node(1, width);
        if (width) record->fields[width - 1].bits = value_for(i, width) + (changed ? 97 : 0);
        wrapper = new_node(2, 2);
        wrapper->fields[0].bits = tag_for(i, period) ^ (changed && !maximum);
        wrapper->fields[1].pointer = record;
    } else {
        wrapper = new_node(2, width + 1);
        wrapper->fields[0].bits = tag_for(i, period) ^ (changed && !maximum);
        if (width) wrapper->fields[width].bits = value_for(i, width) + (changed ? 97 : 0);
    }
    return wrapper;
}
static void store_inline(Buffer *b, size_t index, size_t maximum, size_t period, int changed) {
    size_t width = width_for(index, maximum, period);
    int64_t *slot = (int64_t *)(b->data + index * b->stride);
    memset(slot, 0, b->stride);
    slot[0] = tag_for(index, period) ^ (changed && !maximum);
    if (width) slot[width] = value_for(index, width) + (changed ? 97 : 0);
}
static void append(Buffer *b, int model, size_t i, size_t maximum, size_t period) {
    reserve(b, b->len + 1);
    if (model == 2) store_inline(b, i, maximum, period, 0);
    else ((Node **)b->data)[b->len] = make_variant(model, i, maximum, period, 0);
    ++b->len;
}
static void change_last(Buffer *b, int model, size_t maximum, size_t period) {
    size_t index = b->len - 1;
    if (model == 2) store_inline(b, index, maximum, period, 1);
    else {
        Node **slot = &((Node **)b->data)[index];
        Node *old = *slot;
        *slot = make_variant(model, index, maximum, period, 1);
        release_node(old, model);
    }
}
static int64_t scan(const Buffer *b, int model, size_t maximum, size_t period) {
    int64_t sum = 0;
    for (size_t i = 0; i < b->len; ++i) {
        size_t width = width_for(i, maximum, period);
        int64_t tag, value = 0;
        if (model == 2) {
            const int64_t *slot = (const int64_t *)(b->data + i * b->stride);
            tag = slot[0];
            if (width) value = slot[width];
        } else {
            Node *n = ((Node **)b->data)[i];
            tag = n->fields[0].bits;
            if (width) {
                if (model == 0) value = ((Node *)n->fields[1].pointer)->fields[width - 1].bits;
                else value = n->fields[width].bits;
            }
        }
        sum += tag + value;
    }
    return sum;
}
static Buffer copy_array(const Buffer *b, int model) {
    Buffer copy = {0};
    copy.stride = b->stride;
    reserve(&copy, b->len);
    copy.len = b->len;
    memcpy(copy.data, b->data, b->len * b->stride);
    if (model != 2) {
        for (size_t i = 0; i < b->len; ++i)
            ++((Node **)copy.data)[i]->refs;
    }
    return copy;
}
static void drop_array(Buffer *b, int model) {
    if (model != 2) {
        for (size_t i = 0; i < b->len; ++i)
            release_node(((Node **)b->data)[i], model);
    }
    deallocate(b->data, b->cap * b->stride);
    b->data = NULL;
    b->cap = b->len = 0;
}
static double seconds(void) {
    struct timespec t;
    if (timespec_get(&t, TIME_UTC) != TIME_UTC) abort();
    return (double)t.tv_sec + (double)t.tv_nsec / 1e9;
}
static size_t numeric(const char *arg) {
    char *end = NULL;
    unsigned long long v = strtoull(arg, &end, 10);
    if (!arg[0] || *end || v > SIZE_MAX) abort();
    return (size_t)v;
}
int main(int argc, char **argv) {
    if (argc != 7) {
        fprintf(stderr, "usage: %s current|fused|inline count max_words wide_period scans clones\n", argv[0]);
        return 2;
    }
    int model = strcmp(argv[1], "current") == 0 ? 0 :
                strcmp(argv[1], "fused") == 0 ? 1 :
                strcmp(argv[1], "inline") == 0 ? 2 : -1;
    if (model < 0) return 2;
    size_t n = numeric(argv[2]), max = numeric(argv[3]), period = numeric(argv[4]);
    size_t scans = numeric(argv[5]), clones = numeric(argv[6]);
    if (!n || !period || n > 10000000 || max > 512 || !scans || !clones) return 2;
    Buffer array = {0};
    array.stride = model == 2 ? (max + 1) * sizeof(int64_t) : sizeof(Node *);
    double begin = seconds();
    for (size_t i = 0; i < n; ++i) append(&array, model, i, max, period);
    double after_build = seconds();
    int64_t total = 0;
    for (size_t r = 0; r < scans; ++r) total += scan(&array, model, max, period);
    double after_scan = seconds();
    for (size_t r = 0; r < clones; ++r) {
        Buffer snapshot = copy_array(&array, model);
        /* A CoW-detached clone is updated without mutating the original. */
        change_last(&snapshot, model, max, period);
        total += scan(&snapshot, model, max, period);
        total += scan(&array, model, max, period);
        drop_array(&snapshot, model);
    }
    double after_cow = seconds();
    drop_array(&array, model);
    double after_drop = seconds();
    if (live_bytes) { fprintf(stderr, "storage accounting leak: %" PRIu64 "\n", live_bytes); return 3; }
    int64_t expected = 0;
    for (size_t i = 0; i < n; ++i)
        expected += tag_for(i, period) + (max ? value_for(i, width_for(i, max, period)) : 0);
    expected *= (int64_t)(scans + 2 * clones);
    expected += (int64_t)(clones * (max ? 97 : (tag_for(n - 1, period) ? -1 : 1)));
    if (total != expected) { fprintf(stderr, "checksum mismatch: %" PRId64 " vs %" PRId64 "\n", total, expected); return 4; }
    printf("%s,%" PRId64 ",%.6f,%.6f,%.6f,%.6f,%" PRIu64 ",%" PRIu64 ",%" PRIu64 ",%" PRIu64 "\n",
           argv[1], total, after_build - begin, after_scan - after_build,
           after_cow - after_scan, after_drop - after_cow,
           allocations, reallocations, requested_bytes, peak_bytes);
    return 0;
}
