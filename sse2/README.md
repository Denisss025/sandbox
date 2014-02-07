# sse2

Functions implemented with sse2 instructions.

## checkdigit

Functions to calculate check digits and to verify some identifiers.

* *calc_checkdigit*. Function calculates the weighted sum of a string.
* *check_ogrn*, *check_ogrnip*, *check_inn*. Functions verifies a given string. Return 0 if the string is not a proper organization or entrepreneur identifier.

### Example

```c
	const char val[16];
	printf("Enter OGRN to check: ");
	scanf("%15s", val);
	printf("\nYou've entered %s OGRN\n", check_ogrn(val) ? "correct" : "wrong");
```
