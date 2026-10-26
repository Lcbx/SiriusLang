/* Reference lowering, not required byte-for-byte compiler output. */
#include "sirius_runtime.h"
#include <math.h>
/* Canonical field order is length, x, y (sorted by name). */
typedef struct { double length; double x; double y; } S_Record_length_x_y;
static S_Record_length_x_y Point(double x, double y) {
    return (S_Record_length_x_y){ .x = x, .y = y, .length = sqrt(x*x + y*y) };
}
int main(void) {
    const S_Record_length_x_y p = Point(3.0, 4.0);
    s_print_f64(p.length);
    return 0;
}
