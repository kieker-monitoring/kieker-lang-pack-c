#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>

#include <kieker_trace.h>

/** Declare internal functions necessary for the test. */
extern long long kieker_trace_current_id;
long long kieker_trace_get_next_id();

/** -- tests -- */

void test_kieker_trace_init_id() {
	kieker_trace_init_id();
	long long id = kieker_trace_current_id;
	long long next_id = kieker_trace_get_next_id();
	long long next2_id = kieker_trace_get_next_id();
	assert_int_equal(id, next_id);
	assert_int_equal(next_id +1, next2_id);
}

void test_kieker_trace_get() {
	kieker_trace_init_id();
	long long id = kieker_trace_current_id;
	kieker_trace_t* trace = kieker_trace_get();

	assert_int_equal(trace->trace_id, id);
	assert_int_equal(trace->order_index, -1);
	assert_int_equal(trace->stack, -1);

	kieker_trace_remove(trace);
}

int main(void) {
	const struct CMUnitTest tests[] = {
	cmocka_unit_test(test_kieker_trace_init_id),
	cmocka_unit_test(test_kieker_trace_get), };

	return cmocka_run_group_tests(tests, NULL, NULL);
}
