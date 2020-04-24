#include <stdio.h>
#include "../../libkieker/monitoring/probes/kieker_probe_plain.h"

long long fact(long long n);

int main (int argc, char** argv) {
	kieker_probe_before_operation_record("examples.basic-example", "int main (int argc, char** argv)");
	printf("Hello World\n");

	long long n = 10;
	printf("Factorial number for %lld is %lld\n", n, fact(n));

	kieker_probe_after_operation_record("examples.basic-example", "int main (int argc, char** argv)");

	return 0;
}

long long fact(long long n) {
	kieker_probe_before_operation_record("examples.basic-example", "long long fact(long long n)");

	long long result;

	if (n == 0) {
		result = 1l;
	} else {
		result = n * fact(n-1);
	}

	kieker_probe_after_operation_record("examples.basic-example", "long long fact(long long n)");

	return result;
}
