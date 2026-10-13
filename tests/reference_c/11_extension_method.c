/* Reference lowering, not required byte-for-byte compiler output. */
#include "sirius_runtime.h"
typedef struct { int64_t x; int64_t y; } S_Record_xy;
/* Extension dispatch is resolved statically. */
static S_Record_xy move__xy(S_Record_xy this_value, int64_t dx, int64_t dy) {
    return (S_Record_xy) { .x = this_value.x + dx, .y = this_value.y + dy };
}
int main(void) {
    const S_Record_xy p = { .x = 2, .y = 7 };
    const S_Record_xy q = move__xy(p, 3, 4);
    s_print_i64(q.x);
    s_print_i64(q.y);
    return 0;
}
