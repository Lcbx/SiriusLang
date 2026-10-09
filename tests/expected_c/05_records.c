/* Reference lowering, not required byte-for-byte compiler output. */
#include "sirius_runtime.h"
typedef struct { int64_t x; int64_t y; } S_Record_xy;
static int64_t sum_xy__xy(S_Record_xy p) { return p.x + p.y; }
int main(void) {
    const S_Record_xy p = { .x = 3, .y = 4 };
    S_Record_xy q = p; /* value copy for inline records */
    q.x += 5;
    s_print_i64(sum_xy__xy(p));
    s_print_i64(sum_xy__xy(q));
    return 0;
}
