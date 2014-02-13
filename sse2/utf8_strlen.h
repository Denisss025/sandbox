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

#ifndef SANDBOX_SSE2_UTF8_STRLEN_H__
#define SANDBOX_SSE2_UTF8_STRLEN_H__

#if defined(__cplusplus)
#	include <cstddef>
extern "C" {
#else
#	include <stddef.h>
#endif
/**
 * @brief Count letters in a given string.
 * @param str - given string.
 * @param size - size of the data.
 * @return Number of letters in a string str.
 */
int utf8_strlen_sse2(const char *str, size_t size);
#if defined(__cplusplus)
}  /* extern "C" */
#endif

#endif  /* SANDBOX_SSE2_UTF8_STRLEN_H__ */
