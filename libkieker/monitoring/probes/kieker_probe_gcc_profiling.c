/*
 * probe_gcc_profiling.c
 *
 *  Created on: Apr 17, 2020
 *      Author: reiner
 */

#include "kieker_probe_gcc_profiling.h"

#include <stdio.h>
#include <time.h>

#include <kieker_controller.h>

void __cyg_profile_func_enter (void *this_fn, void *call_site) {
	kieker_probe_before_operation_record("<package structure>", kieker_controller_get_operation_fqn(this_fn));

	fprintf(stderr,"BeforeOperationEvent Time: %ld, Callee %p, Caller %p", time(NULL), this_fn, call_site);

}

void __cyg_profile_func_exit  (void *this_fn, void *call_site) {
	kieker_probe_after_operation_record("<package structure>", kieker_controller_get_operation_fqn(this_fn));

	fprintf(stderr,"AfterOperationEvent Time: %ld, Callee %p, Caller %p", time(NULL), this_fn, call_site);
}
