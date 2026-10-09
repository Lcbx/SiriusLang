/* Reference lowering, not required byte-for-byte compiler output. */
#include "sirius_runtime.h"
/* Two monomorphizations of identity, cached by concrete signature. */
static int64_t identity__i64(int64_t x) { return x; }
static bool identity__bool(bool x) { return x; }
int main(void) {
    s_print_i64(identity__i64(9));
    s_print_bool(identity__bool(true));
    return 0;
}
