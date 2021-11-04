#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <Trace/trace.h>

void init_controller();
trace_hash_t* get_trace(int thread_id);
void remove_trace(trace_hash_t *data);
void deinit_controller();

int get_socket();
void print_kieker_config();


#endif
