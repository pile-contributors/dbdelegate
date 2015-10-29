/**
 * @file dbdelegate-private.h
 * @brief Declarations for DbDelegate class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_DBDELEGATE_PRIVATE_H_INCLUDE
#define GUARD_DBDELEGATE_PRIVATE_H_INCLUDE

#include <dbdelegate/dbdelegate-config.h>

#if 0
#    define DBDELEGATE_DEBUGM printf
#else
#    define DBDELEGATE_DEBUGM black_hole
#endif

#if 0
#    define DBDELEGATE_TRACE_ENTRY printf("DBDELEGATE ENTRY %s in %s[%d]\n", __func__, __FILE__, __LINE__)
#else
#    define DBDELEGATE_TRACE_ENTRY
#endif

#if 0
#    define DBDELEGATE_TRACE_EXIT printf("DBDELEGATE EXIT %s in %s[%d]\n", __func__, __FILE__, __LINE__)
#else
#    define DBDELEGATE_TRACE_EXIT
#endif


static inline void black_hole (...)
{}

#endif // GUARD_DBDELEGATE_PRIVATE_H_INCLUDE
