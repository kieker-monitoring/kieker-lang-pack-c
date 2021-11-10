#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>

#include <kieker_controller.h>

void kieker_controller_configure_variables();
void kieker_controller_setup_buffer();
int kieker_controller_obtain_local_hostname();

/** Declare internal functions necessary for the test. */

/** -- tests -- */
void test_kieker_controller_configure_variables() {
	kieker_controller_configure_variables();
}

void test_kieker_controller_setup_buffer() {
		kieker_controller_setup_buffer();
}

void test_kieker_controller_obtain_local_hostname() {
	kieker_controller_obtain_local_hostname();
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
