/*
 * kieker_probe_plain.h
 *
 *  Created on: Apr 17, 2020
 *      Author: reiner
 */

#ifndef LIBKIEKER_MONITORING_PROBES_KIEKER_PROBE_PLAIN_H_
#define LIBKIEKER_MONITORING_PROBES_KIEKER_PROBE_PLAIN_H_


#include <stdio.h>

#define KIEKER_FLOW_TRACE_METADATA 0
#define KIEKER_FLOW_BEFORE_OPERATION 1
#define KIEKER_FLOW_AFTER_OPERATION 2

void kieker_probe_before_operation_record(const char* class_signature, const char* operation_signature);

void kieker_probe_after_operation_record(const char* class_signature, const char* operation_signature);


#endif /* LIBKIEKER_MONITORING_PROBES_KIEKER_PROBE_PLAIN_H_ */
