#include "sirius_runtime.h"
int main(void) {
    S_Array_i64 *a = s_array_i64_make(1, (int64_t[]){1});
    s_print_i64(s_array_i64_get_checked(a, 9));
    s_array_i64_release(a);
    return 0;
}
