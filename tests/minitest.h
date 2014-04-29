#ifndef SANDBOX_TESTS_MINITEST_H__JDJDJ77879SGE9999
#define SANDBOX_TESTS_MINITEST_H__JDJDJ77879SGE9999

#include <stdio.h>
#include <string.h>

/* #define _mu_assert(file, line, expr) do { if (!(expr)) {} } while (0) */

/*
 * Based on this code: https://gist.github.com/yukioc/236001
 */

#ifndef MU_NOCOLOR
#	define _mu_clr_reset  "\e[0m"
#	define _mu_clr_red    "\e[31m"
#	define _mu_clr_bold   "\e[1m"
#	define _mu_clr_yellow "\e[33m"
#else 
#	define _mu_clr_reset 
#	define _mu_clr_red 
#	define _mu_clr_bold
#	define _mu_clr_yellow
#endif


#define _mu_prn_fail(file, line, msg, expr) printf(                            \
		_mu_clr_bold _mu_clr_red "  Assertion failed:\n" _mu_clr_reset \
		"    %s\n      %s\n      in " _mu_clr_yellow "%s\n" _mu_clr_reset, \
		msg, expr, file ":" #line )
#define _mu_prn_suite(suite) printf("Suite %s\n", suite);
#define _mu_prn_test(test, passed) printf(                             \
	"  Test: %-20s [" _mu_clr_bold "%s" _mu_clr_reset "]\n",     \
	test, (passed)? _mu_clr_yellow "passed" : _mu_clr_red "FAILED")
#define _mu_prn_result() do { if (_muc_failures) {                     \
		printf("### Failed " _mu_clr_bold _mu_clr_red "%d"     \
		_mu_clr_reset " of " _mu_clr_yellow _mu_clr_bold "%d"  \
		_mu_clr_reset " asserts (%d suites, %d tests).\n",     \
		_muc_failures, _muc_asserts, _muc_suites, _muc_tests); \
	} else { printf("### All tests " _mu_clr_yellow _mu_clr_bold   \
		"passed " _mu_clr_reset                                \
		"(%d asserts in %d suites and %d tests).\n",           \
		_muc_asserts, _muc_suites, _muc_tests); } } while (0)

#define _mu_assert(file, line, msg, expr) do { ++_muc_asserts; if (!(expr)) { \
	++_muc_failures; _mu_prn_fail(file, line, msg, #expr); }} while(0)
#define mu_assert(msg, expr) _mu_assert(__FILE__, __LINE__, msg, expr)

#define mu_run_test(_test) do { int f = _muc_failures; ++_muc_tests; \
	test_##_test(); _mu_prn_test(#_test, (f==_muc_failures));    \
	} while (0)
#define mu_run_suite(_suite) do { ++_muc_suites; _mu_prn_suite(#_suite); \
	suite_##_suite(); printf(_mu_clr_reset "\n"); } while(0)


extern int _muc_suites;
extern int _muc_tests;
extern int _muc_failures;
extern int _muc_asserts;

#define mu_main() _mu_main_impl();        \
int _muc_suites   = 0;                    \
int _muc_tests    = 0;                    \
int _muc_failures = 0;                    \
int _muc_asserts  = 0;                    \
int main() {                              \
	int _mu_prg_result;               \
	_mu_prg_result = _mu_main_impl(); \
	_mu_prn_result();                 \
	return _mu_prg_result; }          \
int _mu_main_impl()

#endif /* SANDBOX_TESTS_MINITEST_H__JDJDJ77879SGE9999 */
