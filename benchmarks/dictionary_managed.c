/* Benchmark frequent updates with managed string keys and values.
   Compile against the existing and changed runtime to compare versions. */
#define _POSIX_C_SOURCE 200809L
#include "sirius_runtime.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void copy_string(void *dst, const void *src) {
	*(S_String **)dst = s_string_retain(*(S_String *const *)src);
}
static void drop_string(void *slot) { s_string_release(*(S_String **)slot); }
static uint64_t clock_ns(void) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (uint64_t)ts.tv_sec * 1000000000u + (uint64_t)ts.tv_nsec;
}
int main(void) {
	puts("size,scenario,ns_per_update");
	for (size_t n=1; n<=4096; n*=4) {
		S_Dict *d=s_dict_new(sizeof(S_String *),sizeof(S_String *),
			copy_string,drop_string,copy_string,drop_string,s_hash_string_key,s_eq_string_key);
		S_String **keys=calloc(n,sizeof(*keys));
		if (!keys) abort();
		S_String *value=s_string_from_utf8("a medium-length value that is shared, not copied");
		for (size_t i=0;i<n;i++) {
			char buf[100];
			snprintf(buf,sizeof(buf),"key/%.8zu/this-key-is-deliberately-not-short",i);
			keys[i]=s_string_from_utf8(buf);
			d=s_dict_insert(d,&keys[i],&value);
		}
		const size_t rounds=150000;
		uint64_t t=clock_ns();
		for (size_t i=0;i<rounds;i++) d=s_dict_insert(d,&keys[i%n],&value);
		printf("%zu,unique,%.2f\n",n,(double)(clock_ns()-t)/rounds);
		S_Dict *snapshot=s_dict_retain(d);
		t=clock_ns();
		for (size_t i=0;i<rounds;i++) d=s_dict_insert(d,&keys[i%n],&value);
		printf("%zu,after_snapshot,%.2f\n",n,(double)(clock_ns()-t)/rounds);
		s_dict_release(snapshot);
		s_dict_release(d);
		for (size_t i=0;i<n;i++) s_string_release(keys[i]);
		s_string_release(value);
		free(keys);
	}
}
