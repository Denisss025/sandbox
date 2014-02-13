/* builtin_defs.h - Macroses for builtin functions.
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

#if (_MSC_VER >= 1300)
#	include <intrin.h>
#endif

/* COMPILER? */
#if defined(__clang__) || defined(__llvm__)
#	if !defined(HAS_BUILTINEXPECT)
#		define HAS_BUILTINEXPECT (__has_builtin(__builtin_expect) ? 1 : 0)
#	endif
#	if !defined(HAS_BUILTINCLZ)
#		define HAS_BUILTINCLZ (__has_builtin(__builtin_clz) ? 1 : 0)
#	endif
#	if !defined(HAS_BUILTINCTZ)
#		define HAS_BUILTINCTZ (__has_builtin(__builtin_ctz) ? 1 : 0)
#	endif
#	if !defined(HAS_BUILTINPOPCOUNT)
#		define HAS_BUILTINPOPCOUNT (__has_builtin(__builtin_popcount) ? 1 : 0)
#	endif
#elif (__GNUC__ >= 3)  
#	if !defined(HAS_BUILTINEXPECT)
#		define HAS_BUILTINEXPECT 1
#	endif
#	if ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 4)))
#		define HAS_BUILTINCLZ 1
#		define HAS_BUILTINCTZ 1
#	endif
#	if !defined(HAS_BUILTINCLZ)
#		define HAS_BUILTINCLZ 0
#	endif
#	if !defined(HAS_BUILTINCTZ)
#		define HAS_BUILTINCTZ 0
#	endif
#	if !defined(HAS_BUILTINPOPCOUNT)
#		define HAS_BUILTINPOPCOUNT 1
#	endif
#else
#	if !defined(HAS_BUILTINEXPECT)
#		define HAS_BUILTINEXPECT 0
#	endif
#	if !defined(HAS_BUILTINCLZ)
#		define HAS_BUILTINCLZ 0
#	endif
#	if !defined(HAS_BUILTINPOPCOUNT)
#		define HAS_BUILTINPOPCOUNT 0
#	endif
#endif

#if HAS_BUILTINEXPECT
#	define BUILTIN_EXPECT(expr,exp_val) __builtin_expect((expr),(exp_val))
#else   /* BUILTIN_EXPECT */
#	define BUILTIN_EXPECT(expr,exp_val) (expr)
#endif  /* BUILTIN_EXPECT */

/* LIKELY */
#define BUILTIN_LIKELY(expr) BUILTIN_EXPECT((expr), !0)

/* UNLIKELY */
#define BUILTIN_UNLIKELY(expr) BUILTIN_EXPECT((expr), 0)

#if HAS_BUILTINCLZ
#	define BUILTIN_CLZ(val) __builtin_clz((val))
#else   /* BUILTIN_CLZ */
#	error "Not ready yet!"
#endif  /* BUILTIN_CLZ */

#if HAS_BUILTINCTZ
#	define BUILTIN_CTZ(pos,val) (pos) = __builtin_ctz((val))
#elif defined(__GNUC__) || defined(__llvm__) || defined(__clang__) 
#	define BUILTIN_CTZ(pos, val) asm("bsf %1, %0" : "=r" (pos) : "rm" (val));
#elif (_MSC_VER >= 1300)
#	define BUILTIN_CTZ(pos, val) _BitScanForward(&(pos), (val));
#elif defined(_MSC_VER)
static int _x_BitScanForward(int val) {
	int pos;
	_asm bsf edx, val;
	_asm mov pos, edx;
	return pos;
}
#	define BUILTIN_CTZ(pos, val) (pos) = _x_BitScanForward((val));
#else   /* BUILTIN_CTZ */
#	error "Not ready yet!"
#endif  /* BUILTIN_CTZ */

#if HAS_BUILTINPOPCOUNT
#	define BUILTIN_POPCOUNT(cnt, val) (cnt) = __builtin_popcount((val))
#else   /* BUILTIN_POPCOUNT */
#	error "Not ready yet!"
#endif  /* BUILTIN_POPCOUNT */

#define BUILTIN_CONCAT_impl(a,b) a##b
#define BUILTIN_CONCAT(a,b) BUILTIN_CONCAT_impl(a,b)

#if ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)))
#	define BUILTIN_STATICASSERT(expr, message) _Static_assert((expr), message)
#else
#	define BUILTIN_STATICASSERT(expr, message) \
		{ enum { BUILTIN_CONCAT(assert_line_, __LINE__) = 1/(!!(expr)) }; }
#endif

