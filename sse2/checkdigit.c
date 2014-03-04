/* checkdigit.c - Check digits for russian tax payer identifiers.
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

#include "sse2/checkdigit.h"
#if defined(__SSE2__)
#	include <emmintrin.h>
#endif
#include "details/builtin_defs.h"

#define WRONG_VAL ((unsigned)(~0))

unsigned calc_checkdigit(const char *x, const char *w) {
	unsigned sum;
#if !defined(__SSE2__)
	unsigned i;
	sum = 0u;
	for (i = 0u; i < 16u; ++i) {
		if (BUILTIN_UNLIKELY(!isdigit(x[i]) && w[i])) return WRONG_VAL;

		sum += (x[i] - '0') * w[i];
	}
#else
	int mask;
	__m128i xmm0;
	__m128i xmm1;
	__m128i xmm2;
	__m128i xmm3;
	/* load string into xmm registry */
	xmm0 = _mm_loadu_si128((const __m128i*)x);
	/* load weights array */
	xmm1 = _mm_loadu_si128((const __m128i*)w);

	/* array of '0' */
	xmm2 = _mm_set1_epi8(0x30);
	/* !isdigit(x[i]) */
	/* * x[i] > '9' */
	xmm3 = _mm_cmpgt_epi8(xmm0, _mm_set1_epi8(0x39));
	/* * x[i] < '0' | x[i] > '9' */
	xmm3 = _mm_or_si128(xmm3, _mm_cmplt_epi8(xmm0, xmm2));
	/* w[i] != 0 & !isdigit(x[i]) */
	xmm3 = _mm_andnot_si128(_mm_cmpeq_epi8(xmm1,
			_mm_setzero_si128()), xmm3);

	/* check if !isdigit(x[i]) && w[i] != 0 */
	mask = _mm_movemask_epi8(xmm3);

	/* convert string to array of ints: x[i] - '0' */
	xmm0 = _mm_xor_si128(xmm0, xmm2);
	/* lo-mask */
	xmm2 = _mm_set1_epi16(0x000F);
	/* multiply (lo) arrays */
	xmm2 = _mm_mullo_epi16(_mm_and_si128(xmm2, xmm0), 
			_mm_and_si128(xmm2, xmm1));
	/* hi-mask */
	xmm3 = _mm_set1_epi16(0x0F00);
	/* apply hi-mask */
	xmm0 = _mm_and_si128(xmm0, xmm3);
	xmm1 = _mm_and_si128(xmm1, xmm3);
	/* multiply (hi) arrays */
	xmm0 = _mm_mulhi_epi16(xmm0, xmm1);
	/* shift (lo) left */
	xmm2 = _mm_slli_si128(xmm2, 1);
	/* sum all the items of the array */
	xmm0 = _mm_sad_epu8(xmm0, xmm2);
	
	sum = BUILTIN_UNLIKELY(mask) ? WRONG_VAL 
			: _mm_extract_epi16(xmm0, 0) + _mm_extract_epi16(xmm0, 4);
#endif
	return sum;
}

#define CONCAT_IMPL(a, b) (a##b)
#define CONCAT(a, b) CONCAT_IMPL(a, b)
#define VAR(name) CONCAT(_##name##_, __LINE__)

#define QUIT_ON_WRONG_WSUM(idx, wgt, mod, pos) if (1) {                    \
	unsigned int VAR(sum); VAR(sum) = calc_checkdigit((idx), (wgt));   \
	if (BUILTIN_UNLIKELY(VAR(sum) == WRONG_VAL) ||                     \
			BUILTIN_UNLIKELY((VAR(sum) % (mod) % 10 + '0') !=  \
			(idx)[(pos)])) { return 0; }}

static inline int _check_region(const char *val) {
	return !(val[0] == val[1] && val[0] == '0');
}

int check_inn(const char *inn, size_t len) {
	static const char x_cs[18] = { 
		3, 7, 2, 4, 10, 3, 5, 9, 4, 
		6, 8, 0, 0,  0, 0, 0, 0, 0 };

	const char *m = x_cs;

	if (len == 10u) {
		m += 2;
	} else {
		QUIT_ON_WRONG_WSUM(inn, m + 1, 11, 10);
	}

	QUIT_ON_WRONG_WSUM(inn, m, 11, len-1);
	return _check_region(inn);
}

int check_ogrn(const char *ogrn, size_t len, unsigned int year) {
	static const char x[40] = {
		 0, 10, 20, 30, 40, 50, 60, 70,
		80, 90,  0,  0, 10,  1,  4,  3, 
		12,  9, 10,  1,  4,  3, 12,  9, 
		10,  1, 10,  1, 10,  1, 10,  1, 
		10,  1, 10,  1,  0,  0,  0,  0
	};
	static const char *mul10 = x;
	static const char *x_13  = x + 24;
	static const char *x_15  = x + 10;
	const char *ogrnip = ogrn - 2;

	unsigned int num_year;

	if (len == 13) {
		QUIT_ON_WRONG_WSUM(ogrn, x_13, 11, 12);
	} else {
		QUIT_ON_WRONG_WSUM(ogrnip, x_15, 13, 16);
	}

	num_year = 1999u + mul10[ogrn[1] & 0xF] + (ogrn[2] & 0xF);
	return BUILTIN_LIKELY(num_year > 2000u) && 
			BUILTIN_LIKELY(num_year < year) && 
			BUILTIN_LIKELY(_check_region(ogrn + 3));
}

