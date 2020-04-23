#include <stdio.h>
#include "../../libkieker/monitoring/probes/kieker_probe_plain.h"

int main (int argc, char** argv) {
	kieker_probe_before_operation_record("examples.basic-example", "int main (int argc, char** argv)");
	printf("Hello World\n");
	kieker_probe_after_operation_record("examples.basic-example", "int main (int argc, char** argv)");

	return 0;
}

