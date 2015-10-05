/*
 * MessagePack system dependencies
 *
 * Copyright (C) 2008-2010 FURUHASHI Sadayuki
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
#ifndef MSGPACK_SYSDEP_H__
#define MSGPACK_SYSDEP_H__
#include <qglobal.h>

#if defined(_WIN32) && defined(_MSC_VER) && _MSC_VER >= 1600
#include <stdint.h>
#endif

#ifndef _WIN32
#include <arpa/inet.h>  /* __BYTE_ORDER */
#endif

#if !defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__)
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define __LITTLE_ENDIAN__
#elif __BYTE_ORDER == __BIG_ENDIAN
#define __BIG_ENDIAN__
#elif _WIN32
#define __LITTLE_ENDIAN__
#endif
#endif


#ifdef __LITTLE_ENDIAN__

#ifdef _WIN32
#  if defined(ntohs)
#    define _msgpack_be16(x) ntohs(x)
#  elif defined(_byteswap_ushort) || (defined(_MSC_VER) && _MSC_VER >= 1400)
#    define _msgpack_be16(x) ((uint16_t)_byteswap_ushort((unsigned short)x))
#  else
#    define _msgpack_be16(x) ( \
        ((((quint16)x) <<  8) ) | \
        ((((quint16)x) >>  8) ) )
#  endif
#else
#  define _msgpack_be16(x) ntohs(x)
#endif

#ifdef _WIN32
#  if defined(ntohl)
#    define _msgpack_be32(x) ntohl(x)
#  elif defined(_byteswap_ulong) || (defined(_MSC_VER) && _MSC_VER >= 1400)
#    define _msgpack_be32(x) ((uint32_t)_byteswap_ulong((unsigned long)x))
#  else
#    define _msgpack_be32(x) \
        ( ((((quint32)x) << 24)               ) | \
          ((((quint32)x) <<  8) & 0x00ff0000U ) | \
          ((((quint32)x) >>  8) & 0x0000ff00U ) | \
          ((((quint32)x) >> 24)               ) )
#  endif
#else
#  define _msgpack_be32(x) ntohl(x)
#endif

#if defined(_byteswap_uint64) || (defined(_MSC_VER) && _MSC_VER >= 1400)
#  define _msgpack_be64(x) (_byteswap_uint64(x))
#elif defined(bswap_64)
#  define _msgpack_be64(x) bswap_64(x)
#elif defined(__DARWIN_OSSwapInt64)
#  define _msgpack_be64(x) __DARWIN_OSSwapInt64(x)
#else
#define _msgpack_be64(x) \
    ( ((((quint64)x) << 56)                         ) | \
      ((((quint64)x) << 40) & 0x00ff000000000000ULL ) | \
      ((((quint64)x) << 24) & 0x0000ff0000000000ULL ) | \
      ((((quint64)x) <<  8) & 0x000000ff00000000ULL ) | \
      ((((quint64)x) >>  8) & 0x00000000ff000000ULL ) | \
      ((((quint64)x) >> 24) & 0x0000000000ff0000ULL ) | \
      ((((quint64)x) >> 40) & 0x000000000000ff00ULL ) | \
      ((((quint64)x) >> 56)                         ) )
#endif

#define _msgpack_load16(cast, from) ((cast)( \
        (((quint16)((quint8*)(from))[0]) << 8) | \
        (((quint16)((quint8*)(from))[1])     ) ))

#define _msgpack_load32(cast, from) ((cast)( \
        (((quint32)((quint8*)(from))[0]) << 24) | \
        (((quint32)((quint8*)(from))[1]) << 16) | \
        (((quint32)((quint8*)(from))[2]) <<  8) | \
        (((quint32)((quint8*)(from))[3])      ) ))

#define _msgpack_load64(cast, from) ((cast)( \
        (((quint64)((quint8*)(from))[0]) << 56) | \
        (((quint64)((quint8*)(from))[1]) << 48) | \
        (((quint64)((quint8*)(from))[2]) << 40) | \
        (((quint64)((quint8*)(from))[3]) << 32) | \
        (((quint64)((quint8*)(from))[4]) << 24) | \
        (((quint64)((quint8*)(from))[5]) << 16) | \
        (((quint64)((quint8*)(from))[6]) << 8)  | \
        (((quint64)((quint8*)(from))[7])     )  ))

#else

#define _msgpack_be16(x) (x)
#define _msgpack_be32(x) (x)
#define _msgpack_be64(x) (x)

#define _msgpack_load16(cast, from) ((cast)( \
        (((quint16)((quint8*)from)[0]) << 8) | \
        (((quint16)((quint8*)from)[1])     ) ))

#define _msgpack_load32(cast, from) ((cast)( \
        (((quint32)((quint8*)from)[0]) << 24) | \
        (((quint32)((quint8*)from)[1]) << 16) | \
        (((quint32)((quint8*)from)[2]) <<  8) | \
        (((quint32)((quint8*)from)[3])      ) ))

#define _msgpack_load64(cast, from) ((cast)( \
        (((quint64)((quint8*)from)[0]) << 56) | \
        (((quint64)((quint8*)from)[1]) << 48) | \
        (((quint64)((quint8*)from)[2]) << 40) | \
        (((quint64)((quint8*)from)[3]) << 32) | \
        (((quint64)((quint8*)from)[4]) << 24) | \
        (((quint64)((quint8*)from)[5]) << 16) | \
        (((quint64)((quint8*)from)[6]) << 8)  | \
        (((quint64)((quint8*)from)[7])     )  ))
#endif


#define _msgpack_store16(to, num) \
    do { quint16 val = _msgpack_be16(num); memcpy(to, &val, 2); } while(0)
#define _msgpack_store32(to, num) \
    do { quint32 val = _msgpack_be32(num); memcpy(to, &val, 4); } while(0)
#define _msgpack_store64(to, num) \
    do { quint64 val = _msgpack_be64(num); memcpy(to, &val, 8); } while(0)

/*
#define _msgpack_load16(cast, from) \
    ({ cast val; memcpy(&val, (char*)from, 2); _msgpack_be16(val); })
#define _msgpack_load32(cast, from) \
    ({ cast val; memcpy(&val, (char*)from, 4); _msgpack_be32(val); })
#define _msgpack_load64(cast, from) \
    ({ cast val; memcpy(&val, (char*)from, 8); _msgpack_be64(val); })
*/

#endif /* msgpack/sysdep.h */

