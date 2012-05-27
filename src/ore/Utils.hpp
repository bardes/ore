#ifndef BASE64_HPP
#define BASE64_HPP

#include <string>
#include <vector>

namespace ore
{
    //Typedefs
    typedef unsigned char byte;
    typedef unsigned int uint;

    //Enums
    /**
     * This enum makes it easier to know what a return value means...
     * ERR_[CLASS]_[DESCRIPTION]
     */
    enum RETURN_VALUE {
        SUCCESS = 0,
        ERR_MAP_READ_ERROR,
        ERR_MAP_INVALID_DATA,
        ERR_MAP_ZLIB_ERROR,
        ERR_MAP_INVALID_COMPRESSION_METHOD,
        ERR_MAP_INVALID_TILESET,
        ERR_TILESET_NO_PATH,
        ERR_TILESET_IMAGE_READ_ERROR,
        ERR_UNKNOWN
    };
    
    /**
    * This function takes an encoded string and decodes it into a ore::byte
    * vector. decodedData will be an empty vector on failure.
    **/
    void base64decode(const std::string &encodedString,
                    std::vector<ore::byte> &decodedData);
}
#endif // BASE64_HPP
