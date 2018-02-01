//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief Over-ridable macros for buffer growth factors.
 *
 *  \synopsis
 // TODO: implement
 */

#pragma once

// MACROS
// ------

// Buffer growth factor of 2.0 is the worst possible.
//      https://github.com/facebook/folly/blob/master/folly/docs/FBVector.md#memory-handling
#ifndef CONTAINED_GROWTH_FACTOR_NUMERATOR
#   define CONTAINED_GROWTH_FACTOR_NUMERATOR 3
#endif

#ifndef CONTAINED_GROWTH_FACTOR_DENOMINATOR
#   define CONTAINED_GROWTH_FACTOR_DENOMINATOR 2
#endif

#define CONTAINED_GROWTH_RATIO double(CONTAINED_GROWTH_FACTOR_NUMERATOR) / double(CONTAINED_GROWTH_FACTOR_DENOMINATOR)
