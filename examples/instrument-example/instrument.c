#include <stdio.h>
#include "../../libkieker/monitoring/probes/kieker_probe_plain.h"

long long fact(long long n);

int main (int argc, char** argv) {
	printf("Hello World\n");

	kieker_probe_before_operation_event("class", "function");
	kieker_probe_after_operation_event("class", "function");
	kieker_probe_before_operation_event("class", "function");
	kieker_probe_after_operation_event("class", "function");


	long long n = 10;
	printf("Factorial number for %lld is %lld\n", n, fact(n));

	return 0;
}

long long fact(long long n) {
	long long result;

	if (n == 0) {
		result = 1l;
	} else {
		result = n * fact(n-1);
	}
	return result;
}
