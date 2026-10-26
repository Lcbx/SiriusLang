/* Reference C: tagged unions of structurally typed records retain/drop managed payloads. */
#include "sirius_runtime.h"
typedef struct { S_String *value; } S_Success;
typedef struct { int64_t code; } S_Failure;
typedef struct {
    int tag;
    union { S_Success success; S_Failure failure; } payload;
} S_Result;
static S_Success Success(S_String *value) {
    return (S_Success){ s_string_retain(value) };
}
static S_Failure Failure(int64_t code) { return (S_Failure){code}; }
static S_Result from_success(S_Success payload) { return (S_Result){.tag=0, .payload.success=payload}; }
static S_Result from_failure(S_Failure payload) { return (S_Result){.tag=1, .payload.failure=payload}; }
static void result_drop(S_Result r) { if (r.tag == 0) s_string_release(r.payload.success.value); }
static S_String *describe(S_Result result) {
    switch (result.tag) {
    case 0: return s_string_retain(result.payload.success.value);
    case 1: return s_string_from_utf8("failed");
    }
    s_panic("invalid union tag");
}
int main(void) {
    S_String *msg = s_string_from_utf8("hello");
    S_Success success = Success(msg);
    S_Result arg1 = from_success(Success(success.value));
    S_String *description1 = describe(arg1);
    s_print_string(description1);
    s_string_release(description1);
    result_drop(arg1);
    s_print_string(msg);
    S_Result arg2 = from_failure(Failure(2));
    S_String *description2 = describe(arg2);
    s_print_string(description2);
    s_string_release(description2);
    result_drop(arg2);
    s_string_release(success.value);
    s_string_release(msg);
    return 0;
}
