/* Reference C: structurally identical records have the same canonical layout. */
#include "sirius_runtime.h"
typedef struct { int64_t a; int64_t z; } S_AZ;
static int64_t combine(S_AZ p) { return p.a * 10 + p.z; }
int main(void) {
    const S_AZ first = {.z = 2, .a = 1};
    const S_AZ second = {.a = 3, .z = 4};
    s_print_i64(combine(first));
    s_print_i64(combine(second));
    return 0;
}
