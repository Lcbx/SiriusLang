/* Reference lowering, not required byte-for-byte compiler output. */
#include "sirius_runtime.h"
typedef struct { int64_t x; int64_t y; } S_Record_xy;
typedef struct { int64_t x; int64_t z; } S_Record_xz;
int main(void) {
    const S_Record_xy p = { .x = 1, .y = 2 };
    /* Resolve spread/del/overrides statically, in source order. */
    const S_Record_xz q = { .x = 10, .z = 5 };
    (void)p;
    s_print_i64(q.x + q.z);
    return 0;
}
