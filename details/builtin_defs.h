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
 
/* COMPILER? */
#if defined(__clang__) || defined(__llvm__)
#	if !defined HAS_BUILTINEXPECT
#		define HAS_BUILTINEXPECT (__has_builtin(__builtin_expect) ? 1 : 0)
#	endif
#elif (__GNUC__ > 3)  
#	if !defined HAS_BUILTINEXPECT
#		define HAS_BUILTINEXPECT 1
#	endif
#else
#	if !defined HAS_BUILTINEXPECT
#		define HAS_BUILTINEXPECT 0
#	endif
#endif

#if (HAS_BUILTINEXPECT == 1)
#	define BUILTIN_EXPECT(expr,exp_val) __builtin_expect((expr),(exp_val))
#else   /* BUILTIN_EXPECT */
#	define BUILTIN_EXPECT(expr,exp_val) (expr)
#endif  /* BUILTIN_EXPECT */

/* LIKELY */
#define BUILTIN_LIKELY(expr) BUILTIN_EXPECT((expr), !0)

/* UNLIKELY */
#define BUILTIN_UNLIKELY(expr) BUILTIN_EXPECT((expr), 0)

