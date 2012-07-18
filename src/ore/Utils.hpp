#ifndef BASE64_HPP
#define BASE64_HPP

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
     * This enum makes it easier to know what a return value means. e.g.:
     * ERR_[CLASS]_[DESCRIPTION]
     */
    enum RETURN_VALUE {
        ERR_MAP_READ_ERROR,
        ERR_MAP_INVALID_DATA,
        ERR_MAP_ZLIB_ERROR,
        ERR_MAP_INVALID_COMPRESSION_METHOD,
        ERR_MAP_INVALID_TILESET,
        ERR_TILESET_NO_PATH,
        ERR_TILESET_IMAGE_READ_ERROR,
        INDEX_OUT_OF_RANGE,
        SUCCESS
    };

    /**
    * This function takes an encoded string and decodes it into an ore::uint8
    * vector. decodedData will be an empty vector on failure.
    **/
    void base64decode(const std::string &encodedString,
                    std::vector<ore::uint8> &decodedData);
}
#endif // BASE64_HPP
