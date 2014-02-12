/* utf8.h - Work with UTF-8 strings.
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

#if defined(__cplusplus)
#	include <cstddef>
extern "C" {
#else
#	include <stddef.h>
#endif

/**
 * @brief Check if the string is a utf-8 string.
 * @param str - pointer to string.
 * @param len - length of the string.
 * return \em true if the string has UTF-8 charset.
 */
int is_utf8_str(const char *str, size_t len);

/**
 * @brief Get the length of a given string.
 * @param str - given string.
 * @param size - size of the given data.
 * @return Length of the given string. \em -1 if the given string is not
 *  in UTF-8.
 */
int utf8_strlen(const char *str, size_t size);

#if defined(__cplusplus)
}
#endif

