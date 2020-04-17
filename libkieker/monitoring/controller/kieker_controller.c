/*
 * kieker_controller.c
 *
 *  Created on: Apr 17, 2020
 *      Author: reiner
 */

#include "kieker_controller.h"
#include "kieker_io.h"
#include "kieker_serialize.h"

#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

long kieker_thread_array_size = 0;
kieker_thread_array_entry* kieker_thread_array = NULL;
int kieker_socket = 0;
int kieker_offset = 0;

char* kieker_controller_string_buffer;

/*
 * Return the current time since epoch in milliseconds.
 */
long long kieker_controller_get_time_ms() {
	struct timeval call_time;
	gettimeofday(&call_time, NULL);
	return ((long long) call_time.tv_sec * 1000000) + call_time.tv_usec;
}

/*
 * Find a thread entry and return it. In case no thread entry is present return NULL.
 * If no thread is currently in the list, return NULL
 */
kieker_thread_array_entry* kieker_controller_create_thread_entry() {
	if (kieker_thread_array_size == 0) {
		kieker_thread_array = malloc(sizeof(kieker_thread_array_entry));
	} else {
		kieker_thread_array = realloc(kieker_thread_array, sizeof(kieker_thread_array_entry)*(kieker_thread_array_size+1));
	}

	kieker_thread_array[kieker_thread_array_size].process_id = getpid();
	kieker_thread_array[kieker_thread_array_size].thread_id = gettid();
	kieker_thread_array[kieker_thread_array_size].trace_id = 0;

	kieker_thread_array_size++;

	return &kieker_thread_array[kieker_thread_array_size-1];
}

/*
 * Initialize the kieker controller.
 */
void kieker_controller_initialize() {
	// TODO this must be read from configuration
	char* host = "localhost";
	unsigned short port = 5678;
	kieker_offset = 0;
	kieker_controller_buffer = malloc(8192*sizeof(char));
	kieker_controller_string_buffer = malloc(8192*sizeof(char));

	kieker_socket = kieker_io_client_socket_open (host, port);
	// TODO this must loop over a record registration file
	kieker_offset += kieker_serialize_string(kieker_controller_buffer, kieker_offset, "kieker.common.record.flow.trace.TraceMetadata");
	kieker_offset += kieker_serialize_string(kieker_controller_buffer, kieker_offset, "kieker.common.record.flow.trace.operation.BeforeOperationEvent");
	kieker_offset += kieker_serialize_string(kieker_controller_buffer, kieker_offset, "kieker.common.record.flow.trace.operation.AfterOperationEvent");

	write(kieker_socket, kieker_controller_buffer, kieker_offset);
	kieker_offset = 0;
}

// TODO register shutdown hook

/**
 * Append a string to the send buffer.
 */
void kieker_controller_send_string(const char* string, int id) {
	int length = strlen(string);
	kieker_serialize_int32(kieker_controller_string_buffer, 0, id);
	kieker_serialize_int32(kieker_controller_string_buffer, 4, length);
	strcpy(kieker_controller_string_buffer+4+4, string);

	write(kieker_socket, kieker_controller_string_buffer, length);
}

/**
 * Send all data in the buffer to the stream.
 *
 * length = length of the data stored in the buffer
 */
void kieker_controller_send(int length) {
	write(kieker_socket, kieker_controller_buffer, length);
}

/*
 * Find a thread entry and return it. In case no thread entry is present return NULL.
 * If no thread is currently in the list, return NULL
 */
kieker_thread_array_entry* kieker_controller_get_thread_entry() {
	if (kieker_thread_array_size != 0) {
		pid_t current_thread_id = gettid();
		for (int i=0;i<kieker_thread_array_size;i++) {
			if (kieker_thread_array[i].thread_id == current_thread_id) {
				return &kieker_thread_array[i];
			}
		}

		return kieker_controller_create_thread_entry();
	} else {
		kieker_controller_initialize();
		return kieker_controller_create_thread_entry();
	}
}

/*
 * Return the currently active trace id, if no trace exists it creates an entry.
 * On error -1 is returned.
 *
 * entry = pointer to a array entry
 */
long long kieker_controller_get_trace_id(kieker_thread_array_entry* entry) {
	if (entry != NULL) {
		return ++entry->trace_id;
	} else {
		return -1;
	}
}

/**
 * Lookup the function name and parameters and produce a FQN string from it.
 *
 * Return the complete string or "<lookup failed %p>"
 */
char* kieker_controller_get_operation_fqn(void *function_ptr) {
	char result[16+2+16+1];

	sprintf(result,"<lookup failed %p>");

	return result;
}
