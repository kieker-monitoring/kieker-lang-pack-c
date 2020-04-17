#include <stdio.h>
#include "operation_execution_record.h"

int main (int argc, char** argv) {
	char *buffer = malloc(sizeof(char)*10000);
	kieker_common_record_controlflow_operation_execution_record record;

	record.operationSignature = "brabbel";
	record.sessionId = "1234";
	record.traceId = 4;
	record.tin = 1;
	record.tout = 2;
	record.hostname = "localhost";
	record.eoi = 2;
	record.ess = 1;

	kieker_common_record_controlflow_operation_execution_record_serialize(buffer, 1, 0, record);

	printf("Hello World\n");
	return 0;
}

