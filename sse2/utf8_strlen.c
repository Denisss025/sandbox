/* utf8_strlen.h - Calculate string length for the given utf8 string.
 *
 * Copyright © 2014 Denis Novikov
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/ .
 *
 * Author: Denis Novikov <denisnovikov@garant.ru>
 */

#include "sse2/utf8_strlen.h"
#if defined(__SSE2__)
#	include <emmintrin.h>
#endif
#include "details/builtin_defs.h"

int utf8_strlen_sse2(const char *str, size_t size) {
	int result;
	result = 0;
#if !defined(__SSE2__)
	while (size--) {
		result += ((*str & 0xC0) != 0x80) ? 1 : 0;
		++str;
	}
#else
	BUILTIN_STATICASSERT((sizeof(__m128i) == 16), "Wrong size of MMX registry");

	{ /* align string to 16 bytes */
		size_t i;
		for (i = size & 0xF; i; --i, ++str) {
			if (BUILTIN_UNLIKELY(!*str)) return result;
			result += ((*str & 0xC0) != 0x80) ? 1 : 0;
		}
	}


	{
		__m128i xmm0;
		__m128i xmm1;
		__m128i xmm2;
		__m128i xmm3;
		int mask;
		int pos;
		int i;

		size &= ~0xFLU;
		xmm1 = _mm_set_epi32(0xC0C0C0C0, 0xC0C0C0C0, 0xC0C0C0C0, 0xC0C0C0C0);
		xmm2 = _mm_set_epi32(0x80808080, 0x80808080, 0x80808080, 0x80808080);
		xmm3 = _mm_setzero_si128();
		pos = 0;

		for (; size; size -= 16, str += 16) {
			/* next 16 bytes */
			xmm0 = _mm_loadu_si128((const __m128i*)str);
			/* compare with zeroes */
			if (BUILTIN_UNLIKELY(mask = _mm_movemask_epi8(
						_mm_cmpeq_epi8(xmm0, xmm3)) != 0)) {
				/* find the first zero byte */
				BUILTIN_CTZ(pos, mask);
				size = 16;
			}

			/* check the first two most significant bits of each byte */
			xmm0 = _mm_and_si128(xmm0, xmm1);
			xmm0 = _mm_cmpeq_epi8(xmm0, xmm2);
			mask = _mm_movemask_epi8(xmm0);
			/* update mask if we've got zeroes */
			if (BUILTIN_UNLIKELY(pos)) {
				mask |= (1 << (17 - pos)) - 1;
			}

			/* count non-zero bits */
			BUILTIN_POPCOUNT(i, mask);
			/* count of zero bits is a count of the utf-8 symbols */
			result += 16 - i;
		}
	}
#endif
	return result;
}

