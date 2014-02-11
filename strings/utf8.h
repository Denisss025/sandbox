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
extern "C" {
#endif

#include <stddef.h>

/**
 * @brief Check if the string is a utf-8 string.
 * @param str - pointer to string.
 * @param len - length of the string.
 * return \em true if the string has UTF-8 charset.
 */
int is_utf8_str(const char *str, size_t len);

#if defined(__cplusplus)
}
#endif

