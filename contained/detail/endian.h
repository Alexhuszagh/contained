//  :copyright: (c) 2014-2016 Mathias Panzenböck.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: Public Domain/MIT, see licenses/mit.md for more details.
/**
 *  Cross-platform definitions similar to Linux's `<endian.h>`,
 *  defining routines to detect the system byte-order.
 *
 *  Do not prefix these macros, as they aim to provide replacements
 *  for commonly defined macros on unsupported platforms.
 *
 *  \synopsis
 *      // DETECTION
 *      #define __BYTE_ORDER                implementation-defined
 *      #define BYTE_ORDER                  implementation-defined
 *      #define __BIG_ENDIAN                implementation-defined
 *      #define BIG_ENDIAN                  implementation-defined
 *      #define __LITTLE_ENDIAN             implementation-defined
 *      #define LITTLE_ENDIAN               implementation-defined
 *      #define __PDP_ENDIAN                implementation-defined
 *      #define PDP_ENDIAN                  implementation-defined
 *      #define __FLOAT_WORD_ORDER          implementation-defined
 *      #define FLOAT_WORD_ORDER            implementation-defined
 */

#pragma once

#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64)) && !defined(__WINDOWS__)

#   define __WINDOWS__

#endif

#if defined(__linux__) || defined(__CYGWIN__)

#   include <endian.h>

#elif defined(__APPLE__)

#   include <libkern/OSByteOrder.h>

#   define __BYTE_ORDER    BYTE_ORDER
#   define __BIG_ENDIAN    BIG_ENDIAN
#   define __LITTLE_ENDIAN LITTLE_ENDIAN
#   define __PDP_ENDIAN    PDP_ENDIAN

#elif defined(__OpenBSD__)

#   include <sys/endian.h>

#elif defined(__NetBSD__) || defined(__FreeBSD__) || defined(__DragonFly__)

#   include <sys/endian.h>

#elif defined(__WINDOWS__)

#   include <winsock2.h>
#   include <windows.h>
#   include <ws2tcpip.h>

#   ifndef BIG_ENDIAN
        /* Windows does not always set byte order, such as MSVC */
#       define LITTLE_ENDIAN REG_DWORD_LITTLE_ENDIAN
#       define BIG_ENDIAN REG_DWORD_BIG_ENDIAN
#       if REG_DWORD == REG_DWORD_BIG_ENDIAN
#           define BYTE_ORDER BIG_ENDIAN
#       else
#           define BYTE_ORDER LITTLE_ENDIAN
#       endif

#   endif

#   define __BYTE_ORDER    BYTE_ORDER
#   define __BIG_ENDIAN    BIG_ENDIAN
#   define __LITTLE_ENDIAN LITTLE_ENDIAN
#   define __PDP_ENDIAN    PDP_ENDIAN

#else

#   error "Platform not supported."

#endif

// FLOATS
// ------

#ifndef __FLOAT_WORD_ORDER
    /* Nearly all systems use little endian floats */
    #define __FLOAT_WORD_ORDER  LITTLE_ENDIAN
#endif

#ifndef FLOAT_WORD_ORDER
    #define FLOAT_WORD_ORDER    __FLOAT_WORD_ORDER
#endif
