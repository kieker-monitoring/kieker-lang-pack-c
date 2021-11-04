#include "../../aspectcpp/measurement/measurement.h"

#include <sys/time.h>
#include <stdio.h>

void evaluation() {
	for (int j = 0; j < 1000; j++) {
	
		struct timeval tv;
		gettimeofday(&tv, NULL);
		long long tin = tv.tv_sec * 1000000ULL + tv.tv_usec;
	
		for (int i = 0; i < 10000; i++) {
			dummyFunc();
		}
	
		gettimeofday(&tv, NULL);
		long long tout =  tv.tv_sec * 1000000ULL + tv.tv_usec;
		//printf("total time: %lldus\n", tout - tin);
		printf("%lld\n", tout - tin);
	}
}

void dummyFunc() {}