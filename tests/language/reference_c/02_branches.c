/* Reference lowering, not required byte-for-byte compiler output. */
#include "sirius_runtime.h"
static bool expensive(void) {
    s_print_i64(999);
    return true;
}
int main(void) {
    const int64_t x = 7;
    if (false && expensive()) {
        s_print_i64(111);
    } else if (x >= 5 && x < 9) {
        s_print_i64(222);
    } else {
        s_print_i64(333);
    }
    return 0;
}
