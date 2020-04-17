/*
 * kieker_controller.h
 *
 *  Created on: Apr 17, 2020
 *      Author: reiner
 */

#ifndef LIBKIEKER_MONITORING_CONTROLLER_KIEKER_CONTROLLER_H_
#define LIBKIEKER_MONITORING_CONTROLLER_KIEKER_CONTROLLER_H_

#include <sys/types.h>

/*
 * Global buffer for kieker IO.
 */
char* kieker_controller_buffer = NULL;

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
 * Return the currently active trace id, if no trace exists it creates an entry.
 * On error -1 is returned.
 *
 * entry = pointer to a array entry
 */
long long kieker_controller_get_trace_id(kieker_thread_array_entry* entry);

/**
 * Lookup the function name and parameters and produce a FQN string from it.
 *
 * Return the complete string or "<lookup failed %p>"
 */
char* kieker_controller_get_operation_fqn(void *function_ptr);

/**
 * Append a string to the send buffer.
 */
void kieker_controller_send_string(const char* string);

/**
 * Send all data in the buffer to the stream.
 *
 * length = length of the data stored in the buffer
 */
void kieker_controller_send(int length);

#endif /* LIBKIEKER_MONITORING_CONTROLLER_KIEKER_CONTROLLER_H_ */
