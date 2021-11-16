/*
 * kieker_probe_plain.h
 *
 *  Created on: Apr 17, 2020
 *      Author: reiner
 */

#ifndef LIBKIEKER_MONITORING_PROBES_KIEKER_PROBE_PLAIN_H_
#define LIBKIEKER_MONITORING_PROBES_KIEKER_PROBE_PLAIN_H_

#include <stdio.h>
#include "../controller/kieker_trace.h"

/*
 * Complete before operation probe.
 */
void kieker_probe_before_operation_event(const char* class_signature, const char* operation_signature);

/*
 * Complete after operation probe.
 */
void kieker_probe_after_operation_event(const char* class_signature, const char* operation_signature);


#endif /* LIBKIEKER_MONITORING_PROBES_KIEKER_PROBE_PLAIN_H_ */
