/* Reference lowering: pub value.x is shorthand for pub x = value.x. */
#include "sirius_runtime.h"
typedef struct { int64_t x; int64_t y; } S_XY;
typedef struct { int64_t x; } S_X;
static S_X Wrapped(S_XY value) { return (S_X){.x=value.x}; }
int main(void) {
    const S_XY point = {.x=8, .y=9};
    const S_X wrapped = Wrapped(point);
    s_print_i64(wrapped.x);
    return 0;
}
