/*
 * probe_gcc_profiling.h
 *
 *  Created on: Apr 17, 2020
 *      Author: reiner
 */

#ifndef LIBKIEKER_MONITORING_PROBES_KIEKER_PROBE_GCC_PROFILING_H_
#define LIBKIEKER_MONITORING_PROBES_KIEKER_PROBE_GCC_PROFILING_H_

#include "kieker_probe_plain.h"

/*
 * These two functions are expected by the gcc compiler when -finstrument-functions
 * is used to instrument all functions of a program.
 *
 * You may use -finstrument-functions-exclude-file-list=file,file,… to exclude files
 * from being insrumented. Functions can be excluded via
 * -finstrument-functions-exclude-function-list=sym,sym,…
 *
 * https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html#Instrumentation-Options
 */

void __cyg_profile_func_enter (void *this_fn, void *call_site) __attribute__((no_instrument_function));
void __cyg_profile_func_exit  (void *this_fn, void *call_site) __attribute__((no_instrument_function));


#endif /* LIBKIEKER_MONITORING_PROBES_KIEKER_PROBE_GCC_PROFILING_H_ */
