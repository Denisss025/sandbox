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

#ifndef SANDBOX_SSE2_CHECKDIGIT_H__
#define SANDBOX_SSE2_CHECKDIGIT_H__

#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif
	/**
	 * @brief Get the weighted sum of the string.
	 * @param x - string that contains the identifier (char[16]).
	 * @param w - array of weights for each position  (char[16]).
	 * @return Weigthed sum for the identifier.
	 *
	 * The function converts the array of chars (string) into an
	 * array of unsigned 8-bit ints (uint8_t) and then calculates
	 * the weigthed sum of this array with the function below.
	 * sum = x[0] * w[0] + x[1] * w[1] + ... + x[15] * w[15].
	 * \em Note: w[i] should be in the range [0;20]. Otherwise 
	 * it is possible to get the wrong result.
	 */
	unsigned calc_checkdigit(const char *x, const char *w);

	/**
	 * @brief Check if ogrn is correct.
	 * @param ogrn - String that contains OGRN (13 chars, aligned to 16 bytes).
	 * @return \em true if the OGRN string is correct.
	 */
	int check_ogrn(const char *ogrn);

	/**
	 * @brief Check if ogrnip is correct.
	 * @param ogrnip - String that contains OGRNIP (15 chars, aligned to 16 bytes).
	 * @return \em true if the OGRNIP string is correct.
	 */
	int check_ogrnip(const char *ogrnip);

	/**
	 * @brief Check if inn is correct.
	 * @param inn - String that contains OGRN (10 or 12 chars, aligned to 16 bytes).
	 * @return \em true if the INN string is correct.
	 */
	int check_inn(const char *inn, size_t len);
#if defined(__cplusplus)
}  /* extern "C" */
#endif

#endif  /* SANDBOX_SSE2_CHECKDIGIT_H__ */
