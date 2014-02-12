/* utf8_example.c - simple tests.
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

#include <stdio.h>
#include "strings/utf8.h"

int main() {
	int i;

	const char *test[] = {
		"это простой тест 123 asxd",
		"\xC2test",
		"خوش، دنیا",
		"你好，世界",
		"שלום, עולם",
		"Γεια σας, τον κόσμο",
		"こんにちは、世界中のみなさん",
		NULL
	};

	printf("TEST[0] = %s (length: %d)\n", is_utf8_str(test[0], 0) ? 
			"OK" : "FAIL", utf8_strlen(test[0], 0));
	printf("TEST[1] = %s (length: %d)\n", is_utf8_str(test[1], 0) ? 
			"FAIL" : "OK", utf8_strlen(test[1], 0));
	for (i = 2; test[i]; ++i) {
		printf("TEST[%d] = %s (length: %d)\n", i, is_utf8_str(test[i], 0) ? 
				"OK" : "FAIL", utf8_strlen(test[i], 0));
	}

	return 0;
}

