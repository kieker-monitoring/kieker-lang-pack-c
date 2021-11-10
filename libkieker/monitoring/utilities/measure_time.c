#include "../../monitoring/utilities/measure_time.h"

#include <stdlib.h>

#include "../../monitoring/utilities/kieker_error.h"

/*
 * return current system time in nano seconds
 */
long long measure_time() {
    struct timeval tv;
    int ret = gettimeofday(&tv, NULL);
    if (ret != 0) {
        KIEKER_ERROR_PLAIN("Error while measuring time.");
    }
    return (tv.tv_sec * 1000000ULL + tv.tv_usec) * 1000ULL;
}
