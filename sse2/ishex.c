#include "sse2/ishex.h"
#include <string.h>
#include <stdio.h>
#if defined(__SSE2__)
#	include <emmintrin.h>
#endif
#include <ctype.h>

#include "details/builtin_defs.h"

int ishexstr(const char * str, size_t len) {
	if (!len) len = strlen(str);
	if (BUILTIN_UNLIKELY(!len)) return 0;
#if !defined(__SSE2__)
	{
		const char * end = str + len;
		while (str != end) {
			if (!isxdigit(*str)) return 0;
			++str;
		}
	}
#else
	{
		size_t offs;
		for (offs = len & 0xF; offs; --offs, ++str) {
			if (!isxdigit(*str)) return 0;
		}
	}

	{
		int mask;
		__m128i xmm0;
		__m128i xmm1;
		__m128i xmm2;
		__m128i xmm3; 
		__m128i xmm4;
		__m128i xmm5;
		__m128i xmm6;

		xmm2 = _mm_set1_epi8(0x20);
		xmm3 = _mm_set1_epi8(0x30); 
		xmm4 = _mm_set1_epi8(0x39);
		xmm5 = _mm_set1_epi8(0x61);
		xmm6 = _mm_set1_epi8(0x66);

		for (len &= ~(0xFlu); len; len -= 16, str += 16) {

			xmm0 = _mm_loadu_si128((const __m128i*)str);
			xmm1 = _mm_cmpgt_epi8(xmm0, xmm4);
			xmm0 = _mm_or_si128(
				xmm0, _mm_and_si128(xmm2, xmm1));
			xmm0 = _mm_or_si128(
				_mm_andnot_si128(xmm1, 
					_mm_cmplt_epi8(xmm0, xmm3)), 
				_mm_and_si128(xmm1, _mm_or_si128(
					_mm_cmplt_epi8(xmm0, xmm5),
					_mm_cmpgt_epi8(xmm0, xmm6))));

			mask = _mm_movemask_epi8(xmm0);
			if (BUILTIN_UNLIKELY(mask))
				return 0;
		}
	}
#endif
	return 1;
}

