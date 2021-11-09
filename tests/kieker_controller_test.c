#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>

#include <kieker_controller.h>

/** Declare internal functions necessary for the test. */

/** -- tests -- */

void test_kieker_controller_initialize() {
	kieker_controller_initialize();
	// assert correct setup
}

/*
 * Return the current time since epoch in milliseconds.
 */
void test_kieker_controller_get_time_ms() {
	long long value = kieker_controller_get_time_ms();
	// assert time
}

/*
 * Return the hostname.
 */
void test_kieker_controller_get_hostname() {
	assert_null(kieker_controller_get_hostname());
	kieker_controller_initialize();
	assert_not_null(kieker_controller_get_hostname());
	kieker_controller_finalize();
	assert_null(kieker_controller_get_hostname());
}

void test_kieker_controller_get_buffer() {
	assert_null(kieker_controller_get_buffer());
	kieker_controller_initialize();
	assert_not_null(kieker_controller_get_buffer());
	kieker_controller_finalize();
	assert_null(kieker_controller_get_buffer());
}

/**
 * Lookup the function name and parameters and produce a FQN string from it.
 *
 * Return the complete string or "<lookup failed %p>"
 */
//const char* kieker_controller_get_operation_fqn(void *function_ptr);

int main(void) {
	const struct CMUnitTest tests[] = {
	cmocka_unit_test(test_kieker_controller_initialize),
	cmocka_unit_test(test_kieker_controller_get_time_ms),
	cmocka_unit_test(test_kieker_controller_get_hostname),
	cmocka_unit_test(test_kieker_controller_get_buffer), };

	return cmocka_run_group_tests(tests, NULL, NULL);
}
