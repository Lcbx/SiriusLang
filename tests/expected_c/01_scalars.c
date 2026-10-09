/* Reference lowering, not required byte-for-byte compiler output. */
#include "sirius_runtime.h"
int main(void) {
    const int64_t a = 6;
    int64_t b = 2;
    b += a * 3;
    s_print_i64(b);
    return 0;
}
