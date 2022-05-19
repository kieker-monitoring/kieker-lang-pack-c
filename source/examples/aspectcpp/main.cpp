#include <stdlib.h>
#include <iostream>
#include <omp.h>

#include "../aspectcpp/math/fibs.hh"
#include "../aspectcpp/math/power.h"
#include "../aspectcpp/measurement/measurement.h"
#include "../aspectcpp/string/string_test.h"

void call(int n) {
    std::cout << "calculate fib(" << n << ")" << std::endl;
      int ret;
  #pragma omp parallel num_threads(10)
    {
      std::cout << "started thread " << omp_get_thread_num() << std::endl;
      ret = fib_rec(n);
      ret = power(ret);
    }
    std::cout << "the result is " << ret << std::endl;

    string_test();
		
		evaluation();
}

int main(int argc, char *argv[]) {
	std::cout << "starting fibs..." << std::endl;
    int n = 3;

	if (argc > 1) {
		n = atoi(argv[1]);
	}

    call(n);

	return EXIT_SUCCESS;
}
