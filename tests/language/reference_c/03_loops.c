/* Reference lowering, not required byte-for-byte compiler output. */
#include "sirius_runtime.h"
int main(void) {
    int64_t total = 0;
    for (int64_t i = 0; i < 8; ++i) {
        if (i == 2) continue;
        if (i == 6) break;
        total += i;
    }
    while (total > 10) {
        total -= 3;
    }
    s_print_i64(total);
    return 0;
}
