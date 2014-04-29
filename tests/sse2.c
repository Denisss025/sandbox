#include "tests/minitest.h"
#include "tests/sse2.h"

void suite_SSE2() {
	mu_run_test(ishexstr);
}

int mu_main() {
	mu_run_suite(SSE2);

	return 0;
}

