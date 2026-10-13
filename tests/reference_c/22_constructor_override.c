/* Reference C: pub fields initialized unconditionally, with conditional overrides. */
#include "sirius_runtime.h"
typedef struct { int64_t x; int64_t y; } S_Choice;
static S_Choice Choice(int64_t x, bool flag) {
    int64_t published_x = x;
    if (flag) published_x = 10;
    int64_t published_y = x + 1;
    return (S_Choice){ .x=published_x, .y=published_y };
}
int main(void) {
    S_Choice a = Choice(2, false);
    S_Choice b = Choice(2, true);
    s_print_i64(a.x);
    s_print_i64(b.x);
    s_print_i64(b.y);
    return 0;
}
