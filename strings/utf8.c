/* utf8.c - Work with UTF-8 strings.
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

#include "strings/utf8.h"
#include <stdint.h>
#include <string.h>
#include "details/builtin_defs.h"

typedef const uint8_t cuint8_t;
typedef cuint8_t*     cpuint8_t;

static cuint8_t _is_utf8_trans_keys[] = {
	"\x80\xBF\xA0\xBF\x80\xBF\x80\x9F"
	"\x90\xBF\x80\xBF\x80\x8F\xE0\xED"
	"\xF0\xF4\x80\xC1\xC2\xDF\xE1\xEF"
	"\xF1\xF3\xF5\xFF\x00"
};

static cuint8_t _is_utf8_trans_targs[] = {
	8, 0, 1, 0, 1, 0, 1, 0, 
	3, 0, 3, 0, 3, 0, 2, 4, 
	5, 7, 0, 1, 3, 6, 0, 8, 
	0
};

static cuint8_t is_utf8_final = 8;
static cuint8_t is_utf8_error = 0;

int is_utf8_str(const char *str, size_t len) {
	return utf8_strlen(str, len) != -1;
}

int utf8_strlen(const char *str, size_t size) {
	cpuint8_t s;        /* string        */
	cpuint8_t se;       /* string end    */
	cpuint8_t keys;     /* keys          */
	cpuint8_t lk;       /* left key      */
	cpuint8_t mk;       /* middle key    */
	cpuint8_t rk;       /* right key     */
	uint8_t   cs;       /* current state */
	uint8_t   klen;     /* key length    */
	uint8_t   trans;    /* transfigure   */
	int       i;        /* iterator      */
	int       match;    /* matches       */
	int       result;   /* result        */

	if (!size) size = strlen(str);

	s = (cpuint8_t)str;
	cs = is_utf8_final;
	result = 0;
	for (se = s + size; s != se && cs != is_utf8_error; ++s) {
		trans = (cs - 1) << 1;
		keys = _is_utf8_trans_keys + trans; 
		match = 0;
		
		for (i = cs == is_utf8_final ? 0 : 1; i < 2 && !match; ++i) {
			klen = (cs == is_utf8_final ? 4 : 0) + i;
			for (lk = keys, rk = keys + (klen << i) - 1 - i; 
					lk <= rk && !match;) {
				mk = lk + (((rk - lk) >> i) & ~i);
				if (*s < *mk) rk = mk - 1 - i;
				else if (*s > mk[i]) lk = mk + 1 + i;
				else {
					trans += (uint8_t)((mk - keys) >> i);
					match = 1;
				}
			}
			trans +=  match       ? 0 : klen;
			keys  += (match || i) ? 0 : klen;
		}

		cs = _is_utf8_trans_targs[trans];
		result += cs == is_utf8_final ? 1 : 0;
	}

	return BUILTIN_LIKELY(cs == is_utf8_final) ? result : -1;
}

