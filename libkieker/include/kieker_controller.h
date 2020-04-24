/*
 * kieker_controller.h
 *
 *  Created on: Apr 17, 2020
 *      Author: reiner
 */

#ifndef LIBKIEKER_MONITORING_CONTROLLER_KIEKER_CONTROLLER_H_
#define LIBKIEKER_MONITORING_CONTROLLER_KIEKER_CONTROLLER_H_

#include <sys/types.h>
#include <stdlib.h>

typedef struct __kieker_thread_array_entry {
	pid_t thread_id;
	pid_t process_id;
	long long trace_id;
	long long order_index;
} kieker_thread_array_entry;

/*
 * Return the current time since epoch in milliseconds.
 */
long long kieker_controller_get_time_ms();

/*
 * Find a thread entry and return it. In case no thread entry is present return NULL.
 * If no thread is currently in the list, return NULL
 */
kieker_thread_array_entry* kieker_controller_get_thread_entry();

/*
 * Return the hostname.
 */
const char* kieker_controller_get_hostname();

/*
 * Return the buffer.
 */
char* kieker_controller_get_buffer();

/**
 * Lookup the function name and parameters and produce a FQN string from it.
 *
 * Return the complete string or "<lookup failed %p>"
 */
const char* kieker_controller_get_operation_fqn(void *function_ptr);

/**
 * Append a string to the send buffer.
 */
void kieker_controller_send_string(const char* string, int id);

/**
 * Send all data in the buffer to the stream.
 *
 * length = length of the data stored in the buffer
 */
void kieker_controller_send(int length);

/**
 * Write the id and logging timestamp to the buffer.
 *
 * id = id of the record type.
 * offset = offset in the buffer
 */
int kieker_monitoring_controller_prefix_serialize(int id, int offset);

#endif /* LIBKIEKER_MONITORING_CONTROLLER_KIEKER_CONTROLLER_H_ */
