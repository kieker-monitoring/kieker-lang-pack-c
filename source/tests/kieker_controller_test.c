#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>

#include "kieker_controller_internal.h"

extern kieker_controller_state_t kieker_controller;

/** Declare internal functions necessary for the test. */

/** -- tests -- */
void test_kieker_controller_configure_variables() {
	kieker_controller_configure_variables();
	assert_string_equal(kieker_controller.remote_hostname, KIEKER_DEFAULT_REMOTE_HOSTNAME);
	assert_int_equal(kieker_controller.remote_port, KIEKER_DEFAULT_REMOTE_PORT);
	assert_string_equal(kieker_controller.event_types_filename, KIEKER_DEFAULT_EVENT_TYPE_FILENAME);
}

void test_kieker_controller_setup_buffer() {
	kieker_controller_setup_buffer();
	assert_non_null(kieker_controller.buffer);
}

void test_kieker_controller_obtain_local_hostname() {
	if (kieker_controller_obtain_local_hostname())
		assert_non_null(kieker_controller.local_hostname);
	else
		assert_string_equal(kieker_controller.local_hostname, "<failed>");
}


/**
 * Lookup the function name and parameters and produce a FQN string from it.
 *
 * Return the complete string or "<lookup failed %p>"
 */
//const char* kieker_controller_get_operation_fqn(void *function_ptr);

int main(void) {
	const struct CMUnitTest tests[] = {
	cmocka_unit_test(test_kieker_controller_configure_variables),
	cmocka_unit_test(test_kieker_controller_setup_buffer),
	cmocka_unit_test(test_kieker_controller_obtain_local_hostname),};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
