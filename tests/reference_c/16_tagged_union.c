/* Reference lowering, not required byte-for-byte compiler output. */
#include "sirius_runtime.h"
/* Record types are canonical by their fields, not constructor names. */
typedef struct { int64_t value; } S_Record_value;
typedef struct { int64_t code; } S_Record_code;
static S_Record_value Success(int64_t value) {
    return (S_Record_value){.value = value};
}
static S_Record_code Failure(int64_t code) {
    return (S_Record_code){.code = code};
}
typedef enum { S_Success, S_Failure } S_Result_Tag;
typedef struct {
    S_Result_Tag tag;
    union { S_Record_value success; S_Record_code failure; } data;
} S_Result;
/* Implicit widening from a constructor result to the Result union. */
static S_Result result_from_success(S_Record_value payload) {
    return (S_Result){.tag=S_Success, .data.success=payload};
}
static S_Result result_from_failure(S_Record_code payload) {
    return (S_Result){.tag=S_Failure, .data.failure=payload};
}
static int64_t unwrap(S_Result result) {
    switch (result.tag) {
    case S_Success: {
        const S_Record_value payload = result.data.success;
        return payload.value;
    }
    case S_Failure: {
        const S_Record_code payload = result.data.failure;
        return -payload.code;
    }
    }
    s_panic("invalid union tag");
}
int main(void) {
    s_print_i64(unwrap(result_from_success(Success(7))));
    s_print_i64(unwrap(result_from_failure(Failure(2))));
    return 0;
}
