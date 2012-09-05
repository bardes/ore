#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

#include <sys/types.h>

namespace ore
{
    //Typedefs
    typedef int8_t  int8;
    typedef int16_t int16;
    typedef int32_t int32;
    typedef int64_t int64;

    typedef u_int8_t  uint8;
    typedef u_int16_t uint16;
    typedef u_int32_t uint32;
    typedef u_int64_t uint64;

    //Enums
    /**
     * Values to be thrown during exceptions.
     */
    enum EXCEPTION {
        BAD_ALLOC,
        INDEX_OUT_OF_RANGE,
        INVALID_COMPRESSION_METHOD,
        INVALID_MAP_DATA,
        INVALID_TILESET,
        IMAGE_READ_ERROR,
        NO_PATH,
        MAP_READ_ERROR,
        TILESET_COLLISION,
        ZLIB_ERROR
    };
}
#endif // UTILS_HPP
