/*
 * Experimental one-allocation tagged-union model. NOT the Sirius runtime.
 *
 * The wrapper and its record payload have separate ARC counts, but share an
 * allocation. An extracted record can outlive the wrapper. The sentinel is
 * safe only for fixed-size record/union objects; production arrays grow.
 * Run --check under ASan/UBSan before interpreting benchmark results.
 */
#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Node Node;
typedef struct Text Text;
typedef struct Numbers Numbers;
typedef union { int64_t bits; Node *node; Text *text; Numbers *numbers; } Field;
struct Node { int64_t type_id; size_t refs, length, capacity; Field fields[]; };
struct Text { size_t refs, length; char bytes[]; };
struct Numbers { size_t refs, length; int64_t values[]; };

enum { SMALL = 1, WIDE = 2, MANAGED = 3, VARIANT = 4, EMPTY = 5 };
/* Never use this sentinel on a resizable collection. */
#define FUSED SIZE_MAX
static uint64_t allocations, deallocations, requested_bytes, live_bytes, peak_bytes;
static void *allocate(size_t size) {
    void *p = calloc(1, size);
    if (!p) abort();
    ++allocations; requested_bytes += size; live_bytes += size;
    if (live_bytes > peak_bytes) peak_bytes = live_bytes;
    return p;
}
static void deallocate(void *p, size_t size) {
    assert(p && live_bytes >= size);
    ++deallocations; live_bytes -= size; free(p);
}
static size_t node_bytes(size_t length) {
    assert(length <= (SIZE_MAX - sizeof(Node)) / sizeof(Field));
    return sizeof(Node) + length * sizeof(Field);
}
static Node *node_new(int64_t kind, size_t length) {
    Node *p = allocate(node_bytes(length));
    p->type_id = kind; p->refs = 1; p->length = p->capacity = length;
    return p;
}
static Node *retain(Node *p) { assert(p && p->refs); ++p->refs; return p; }
static Text *text_new(const char *data) {
    size_t len = strlen(data), bytes = sizeof(Text) + len + 1;
    Text *p = allocate(bytes);
    p->refs = 1; p->length = len;
    memcpy(p->bytes, data, len + 1);
    return p;
}
static Text *text_retain(Text *p) { assert(p && p->refs); ++p->refs; return p; }
static void text_release(Text *p) {
    if (p && --p->refs == 0) deallocate(p, sizeof(Text) + p->length + 1);
}
static Numbers *numbers_new(int64_t a, int64_t b) {
    Numbers *p = allocate(sizeof(Numbers) + 2 * sizeof(int64_t));
    p->refs = 1; p->length = 2; p->values[0] = a; p->values[1] = b;
    return p;
}
static Numbers *numbers_retain(Numbers *p) { assert(p && p->refs); ++p->refs; return p; }
static void numbers_release(Numbers *p) {
    if (p && --p->refs == 0) deallocate(p, sizeof(Numbers) + p->length * sizeof(int64_t));
}
/* Wrapper starts at the allocation base, payload immediately after 2 slots. */
static size_t wrapper_bytes(void) { return node_bytes(2); }
static Node *fused_payload(Node *wrapper) {
    return (Node *)((unsigned char *)wrapper + wrapper_bytes());
}
static Node *payload_owner(Node *payload) {
    return (Node *)((unsigned char *)payload - wrapper_bytes());
}
/* Return a wrapper and embedded payload with exactly one wrapper-owned credit. */
static Node *new_fused(int64_t payload_type, size_t fields, int64_t tag) {
    assert(payload_type == EMPTY || payload_type == SMALL || payload_type == WIDE || payload_type == MANAGED);
    Node *wrapper = allocate(wrapper_bytes() + node_bytes(fields));
    Node *payload = fused_payload(wrapper);
    wrapper->type_id = VARIANT; wrapper->refs = 1;
    wrapper->length = 2; wrapper->capacity = FUSED;
    wrapper->fields[0].bits = tag; wrapper->fields[1].node = payload;
    payload->type_id = payload_type; payload->refs = 1;
    payload->length = fields; payload->capacity = FUSED;
    return wrapper;
}
static void release(Node *p) {
    if (!p || --p->refs) return;
    switch (p->type_id) {
        case EMPTY: case SMALL: case WIDE: break;
        case MANAGED:
            text_release(p->fields[1].text);
            numbers_release(p->fields[2].numbers);
            break;
        case VARIANT:
            if (p->capacity == FUSED) {
                /* The payload may free the slab right here. Never touch p after. */
                release(p->fields[1].node);
                return;
            }
            release(p->fields[1].node);
            break;
        default: abort();
    }
    if (p->capacity == FUSED) {
        /* Wrapper owns a payload reference: zero payload refs implies wrapper
         * already reached zero. This is the last live view of the allocation. */
        Node *owner = payload_owner(p);
        assert(owner->type_id == VARIANT && owner->refs == 0);
        deallocate(owner, wrapper_bytes() + node_bytes(p->length));
    } else {
        deallocate(p, node_bytes(p->capacity));
    }
}
static Node *make_variant(int fused, size_t index, int managed, int changed) {
    const int wide = (index & 1) != 0;
    const int64_t type = !wide ? SMALL : managed ? MANAGED : WIDE;
    const size_t length = !wide ? 1 : managed ? 3 : 6;
    Node *wrapper = fused ? new_fused(type, length, wide) : node_new(VARIANT, 2);
    Node *payload = fused ? fused_payload(wrapper) : node_new(type, length);
    payload->fields[0].bits = (int64_t)index + (changed ? 19 : 0);
    if (type == WIDE) {
        for (size_t j = 1; j < length; ++j)
            payload->fields[j].bits = (int64_t)(index + j);
    } else if (type == MANAGED) {
        payload->fields[1].text = text_new(changed ? "changed" : "hello");
        payload->fields[2].numbers = numbers_new((int64_t)index, (int64_t)index + 3);
    }
    if (!fused) {
        wrapper->fields[0].bits = wide;
        wrapper->fields[1].node = payload; /* transfer record ownership */
    }
    return wrapper;
}
/* Mirrors UNION_GET: extraction retains a record and may outlive its union. */
static Node *extract(Node *wrapper) {
    assert(wrapper->type_id == VARIANT);
    return retain(wrapper->fields[1].node);
}
/* Mirrors CoW record uniqueness: copying managed fields acquires new ARC credits. */
static Node *unique_owned(Node *original) {
    if (original->refs == 1) return original;
    Node *copy = node_new(original->type_id, original->length);
    memcpy(copy->fields, original->fields, original->length * sizeof(Field));
    if (copy->type_id == MANAGED) {
        text_retain(copy->fields[1].text);
        numbers_retain(copy->fields[2].numbers);
    }
    release(original);
    return copy;
}
static int64_t inspect(Node *wrapper) {
    Node *payload = wrapper->fields[1].node;
    const int64_t base = payload->fields[0].bits + wrapper->fields[0].bits;
    if (payload->type_id == MANAGED)
        return base + (int64_t)payload->fields[1].text->length +
               payload->fields[2].numbers->values[1];
    if (payload->type_id == WIDE) return base + payload->fields[5].bits;
    return base;
}
typedef struct { Node **values; size_t length; } Batch;
static Batch batch_new(size_t size) {
    Batch b = { allocate(size * sizeof(Node *)), size }; return b;
}
static Batch batch_clone(const Batch *original) {
    Batch clone = batch_new(original->length);
    for (size_t i = 0; i < clone.length; ++i)
        clone.values[i] = retain(original->values[i]);
    return clone;
}
static int64_t scan(const Batch *array) {
    int64_t sum = 0;
    for (size_t i = 0; i < array->length; ++i) sum += inspect(array->values[i]);
    return sum;
}
static void batch_release(Batch *batch) {
    for (size_t i = 0; i < batch->length; ++i) release(batch->values[i]);
    deallocate(batch->values, batch->length * sizeof(Node *));
    batch->values = NULL; batch->length = 0;
}
static double clock_seconds(void) {
    struct timespec t;
    if (timespec_get(&t, TIME_UTC) != TIME_UTC) abort();
    return (double)t.tv_sec + (double)t.tv_nsec / 1000000000.0;
}
static void check_zero(void) {
    if (live_bytes || allocations != deallocations) {
        fprintf(stderr, "LEAK: bytes=%" PRIu64 " allocs=%" PRIu64 " frees=%" PRIu64 "\n",
                live_bytes, allocations, deallocations);
        abort();
    }
}
static void check_lifetimes(int fused) {
    /* An extracted managed record survives release of its union. */
    Node *w = make_variant(fused, 3, 1, 0);
    Node *escaped = extract(w);
    Text *text = text_retain(escaped->fields[1].text);
    Numbers *numbers = numbers_retain(escaped->fields[2].numbers);
    release(w);
    assert(strcmp(escaped->fields[1].text->bytes, "hello") == 0);
    assert(escaped->fields[2].numbers->values[0] == 3);
    /* Record CoW while wrapper is gone, with an independent record alias. */
    Node *alias = retain(escaped);
    Node *changed = unique_owned(escaped);
    text_release(changed->fields[1].text);
    changed->fields[1].text = text_new("new");
    assert(strcmp(alias->fields[1].text->bytes, "hello") == 0);
    assert(strcmp(changed->fields[1].text->bytes, "new") == 0);
    release(changed); release(alias);
    assert(strcmp(text->bytes, "hello") == 0);
    assert(numbers->values[1] == 6);
    text_release(text); numbers_release(numbers);
    /* Detaching an extracted record must not modify the live wrapper. */
    w = make_variant(fused, 3, 1, 0);
    changed = unique_owned(extract(w));
    text_release(changed->fields[1].text);
    changed->fields[1].text = text_new("other");
    assert(strcmp(w->fields[1].node->fields[1].text->bytes, "hello") == 0);
    assert(strcmp(changed->fields[1].text->bytes, "other") == 0);
    release(w); release(changed);
    /* Mixed-payload unions may also contain an empty-record alternative. */
    if (fused) {
        w = new_fused(EMPTY, 0, 7);
    } else {
        w = node_new(VARIANT, 2);
        w->fields[0].bits = 7;
        w->fields[1].node = node_new(EMPTY, 0);
    }
    escaped = extract(w);
    release(w);
    assert(escaped->length == 0);
    release(escaped);
    /* An extracted record outlives the wrapper's last CoW array snapshot. */
    Batch original = batch_new(2);
    original.values[0] = make_variant(fused, 0, 1, 0);
    original.values[1] = make_variant(fused, 1, 1, 0);
    Batch snapshot = batch_clone(&original);
    Node *independent = extract(original.values[1]);
    release(snapshot.values[1]);
    snapshot.values[1] = make_variant(fused, 1, 1, 1);
    assert(scan(&snapshot) != scan(&original));
    batch_release(&snapshot); batch_release(&original);
    assert(independent->fields[0].bits == 1);
    assert(strcmp(independent->fields[1].text->bytes, "hello") == 0);
    release(independent);
    /* A fused wrapper with several ARC owners drops the payload just once. */
    w = make_variant(fused, 0, 0, 0);
    Node *w2 = retain(w);
    release(w); release(w2);
    check_zero();
}
static size_t number(const char *s) {
    char *end = NULL;
    unsigned long long n = strtoull(s, &end, 10);
    if (!s[0] || *end || n > SIZE_MAX) abort();
    return (size_t)n;
}
int main(int argc, char **argv) {
    if (argc == 2 && strcmp(argv[1], "--check") == 0) {
        check_lifetimes(0); check_lifetimes(1);
        puts("baseline and fused lifetime/CoW/managed cases PASS");
        return 0;
    }
    if (argc != 6) {
        fprintf(stderr, "usage: %s current|fused count scalar|managed scans clones\n", argv[0]);
        return 2;
    }
    int fused = strcmp(argv[1], "fused") == 0;
    if (!fused && strcmp(argv[1], "current") != 0) return 2;
    int managed = strcmp(argv[3], "managed") == 0;
    if (!managed && strcmp(argv[3], "scalar") != 0) return 2;
    size_t n = number(argv[2]), scans = number(argv[4]), clones = number(argv[5]);
    if (!n || n > 1000000 || !scans || !clones) return 2;
    double start = clock_seconds();
    Batch array = batch_new(n);
    for (size_t i = 0; i < n; ++i) array.values[i] = make_variant(fused, i, managed, 0);
    double built = clock_seconds();
    int64_t result = 0;
    for (size_t j = 0; j < scans; ++j) result += scan(&array);
    double scanned = clock_seconds();
    for (size_t j = 0; j < clones; ++j) {
        Batch snapshot = batch_clone(&array);
        release(snapshot.values[n - 1]);
        snapshot.values[n - 1] = make_variant(fused, n - 1, managed, 1);
        result += scan(&snapshot) + scan(&array);
        batch_release(&snapshot);
    }
    double cloned = clock_seconds();
    /* A subset of extracted records must survive the wrapper/array release. */
    size_t escaped_count = (n + 63) / 64;
    Node **escaped = allocate(escaped_count * sizeof(Node *));
    for (size_t i = 0; i < escaped_count; ++i)
        escaped[i] = extract(array.values[i * 64]);
    batch_release(&array);
    for (size_t i = 0; i < escaped_count; ++i) {
        result += escaped[i]->fields[0].bits;
        release(escaped[i]);
    }
    deallocate(escaped, escaped_count * sizeof(Node *));
    double dropped = clock_seconds();
    check_zero();
    printf("%s,%s,%" PRId64 ",%.6f,%.6f,%.6f,%.6f,%" PRIu64 ",%" PRIu64 ",%" PRIu64 "\n",
           argv[1], argv[3], result, built - start, scanned - built,
           cloned - scanned, dropped - cloned, allocations, requested_bytes, peak_bytes);
    return 0;
}
