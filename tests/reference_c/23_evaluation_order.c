/* Reference C: evaluated operands, fields, arguments and while tests retain source order. */
#include "sirius_runtime.h"
typedef struct { int64_t a; int64_t z; } S_AZ;
static int64_t tick(int64_t x) { s_print_i64(x); return x; }
static int64_t add(int64_t a, int64_t b) { return a + b; }
static bool condition(int64_t n) { s_print_i64(n); return n < 2; }
int main(void) {
    int64_t first = tick(1);
    int64_t second = tick(2);
    s_print_i64(add(first, second));
    int64_t third = tick(3);
    int64_t fourth = tick(4);
    s_print_i64(third + fourth);
    int64_t z = tick(5);
    int64_t a = tick(6);
    S_AZ p = {.z=z, .a=a};
    s_print_i64(p.z + p.a);
    int64_t n = 0;
    while (condition(n)) n += 1;
    return 0;
}
