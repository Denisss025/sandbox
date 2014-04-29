#include "tests/minitest.h"
#include "tests/sse2.h"
#include "sse2/ishex.h"

void test_ishexstr() {
	mu_assert("Empty string", !ishexstr("", 0)); 
	mu_assert("Hex string without length defined", ishexstr("01234567890abcdefABCDEF", 0));
	mu_assert("Hex string with length", ishexstr("0123", 4));
	mu_assert("Hex string with wrong length", !ishexstr("0123", 5));
	mu_assert("Non-hex string", !ishexstr("This is a bad test", 0));
	mu_assert("First letter is not hex", !ishexstr("G555", 4));
	mu_assert("Last letter is not hex", !ishexstr("ABC`", 4));
	mu_assert("UUID",         ishexstr("76c47f6308624901a6a4ca75da5b2b77", 32));
	mu_assert("32 digits",    ishexstr("71324567890123456089012345679801", 32));
	mu_assert("Contains /",  !ishexstr("/1324567890123456789012345679801", 32));
	mu_assert("Contains :",  !ishexstr("01324567890123456:89012345679801", 32));
	mu_assert("32 hex chars", ishexstr("fbcdefABCDEFfedecdAdfCADBabcdefF", 32));
	mu_assert("Contains G",  !ishexstr("GbcdefABCDEFfedecbAdfCADBabcdefF", 32));
	mu_assert("Contains @",  !ishexstr("BbcdefABCDEFfedec@AdfCADBabcdefF", 32));
	mu_assert("Smaller size", ishexstr("BbcdefABCDEFfedec@AdfCADBabcdefF", 16));
}

