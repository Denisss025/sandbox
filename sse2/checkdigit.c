/* checkdigit.h - Check digits for russian tax payer identifiers.
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
 
#include "checkdigit.h"

#include <string.h>
#if defined(__SSE2__)
#	include <emmintrin.h>
#endif

#define WRONG_VAL ((unsigned)(~0))

#if defined(__GNUC__)
#	define BUILTIN_EXPECT(val,exp) __builtin_expect((val),(exp))
#else   /* BUILTIN_EXPECT */
#define BUILTIN_EXPECT(val,exp) (val)
#endif  /* BUILTIN_EXPECT */

unsigned calc_checkdigit(const char *x, const char *w) {
	unsigned sum;
#if !defined(__SSE2__)
	unsigned i;
	sum = 0u;
	for (i = 0u; i < 16u; ++i) {
		if (BUILTIN_EXPECT(!isdigit(x[i]) && w[i],0)) return WRONG_VAL;

		sum += (x[i] - '0') * w[i];
	}
#else
	__m128i xmm0;
	__m128i xmm1;
	__m128i xmm2;
	__m128i xmm3;
	/* load string into xmm registry */
	xmm0 = _mm_loadu_si128((const __m128i*)x);
	/* load control sum array */
	xmm1 = _mm_loadu_si128((const __m128i*)w);

	/* array of zeros */
	xmm2 = _mm_set_epi32(0x30303030, 0x30303030, 0x30303030, 0x30303030);
	/* !isdigit(x[i]) */
	/* * x[i] > '9' */
	xmm3 = _mm_cmpgt_epi8(xmm0, _mm_set_epi32(0x39393939, 0x39393939, 
			0x39393939, 0x39393939));
	/* * x[i] < '0' | x[i] > '9' */
	xmm3 = _mm_or_si128(xmm3, _mm_cmplt_epi8(xmm0, xmm2));
	/* w[i] != 0 & !isdigit(x[i]) */
	xmm3 = _mm_andnot_si128(_mm_cmpeq_epi8(xmm1,
			_mm_setzero_si128()), xmm3);

	/* check if !isdigit(x[i]) && w[i] != 0 */
	if (BUILTIN_EXPECT(_mm_movemask_epi8(xmm3), 0) != 0) return WRONG_VAL;

	/* convert string to array of ints: x[i] - '0' */
	xmm0 = _mm_sub_epi8(xmm0, xmm2);
	/* lo-mask */
	xmm2 = _mm_set_epi32(0x000F000F, 0x000F000F, 0x000F000F, 0x000F000F);
	/* multiply (lo) arrays */
	xmm2 = _mm_mullo_epi16(_mm_and_si128(xmm2, xmm0), 
			_mm_and_si128(xmm2, xmm1));
	/* hi-mask */
	xmm3 = _mm_set_epi32(0x0F000F00, 0x0F000F00, 0x0F000F00, 0x0F000F00);
	/* apply hi-mask */
	xmm0 = _mm_and_si128(xmm0, xmm3);
	xmm1 = _mm_and_si128(xmm1, xmm3);
	/* multiply (hi) arrays */
	xmm0 = _mm_mulhi_epi16(xmm0, xmm1);
	/* shift (lo) left */
	xmm2 = _mm_slli_si128(xmm2, 1);
	/* sum all the items of the array */
	xmm0 = _mm_sad_epu8(xmm0, xmm2);
	sum = _mm_extract_epi16(xmm0, 0) + _mm_extract_epi16(xmm0, 4);
#endif
	return sum;
}

int check_inn(const char *inn, size_t len) {
	const char x_cs[18] = { 
		3, 7, 2, 4, 10, 3, 5, 9, 4, 
		6, 8, 0, 0,  0, 0, 0, 0, 0 };

	const char *m = x_cs;
	unsigned sum;

	if (len == 10u) {
		m += 2;
	} else {
		sum = calc_checkdigit(inn, m + 1);
		if (sum == WRONG_VAL) return 0;
		if ((sum % 11 % 10 + '0') != inn[10]) return 0;
	}

	sum = calc_checkdigit(inn, m);
	if (sum == WRONG_VAL) return 0;
	return inn[len-1] == (sum % 11 % 10 + '0') ? 1 : 0;
}

int check_ogrn(const char *ogrn) {
	const char x_13[16] = {
		 10,  1, 10,  1, 10,  1, 10,  1,
		 10,  1, 10,  1,  0,  0,  0,  0 };

	unsigned sum;
	sum = calc_checkdigit(ogrn, x_13);
	if (sum == WRONG_VAL) return 0;
	return ogrn[12] == (sum % 11 % 10 + '0') ? 1 : 0;
}

int check_ogrnip(const char *ogrnip) {
 	const char x_15[16] = { 
		10,  1,  4,  3, 12,  9, 10,  1, 
		 4,  3, 12,  9, 10,  1,  0,  0 };

	unsigned sum;
	sum = calc_checkdigit(ogrnip, x_15);
	if (sum == WRONG_VAL) return 0;

	return ogrnip[14] == (sum % 13 % 10 + '0') ? 1 : 0;
}

