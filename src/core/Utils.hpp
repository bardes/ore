#ifndef BASE64_HPP
#define BASE64_HPP

#include <string>
#include <vector>

namespace ore
{
    //Constants

    //Typedefs
    typedef unsigned char Byte;

    //Enums
    enum RETURN_VALUES {
        SUCCESS = 0,
        ERR_FILE_READ_ERROR,
        ERR_INVALID_MAP_DATA,
        ERR_ZLIB_ERROR,
        ERR_INVALID_COMPRESSION_METHOD,
        ERR_INVALID_TILESET,
        ERR_NO_PATH,
        ERR_NO_SDL_VIDEO,
        ERR_UNKNOWN
    };
    
    /**
    * This function takes an encoded string and decodes it into a unsigned char
    * vector. decodedData will be an empty vector on failure.
    **/
    void base64decode(const std::string &encodedString,
                    std::vector<Byte> &decodedData);
}
#endif // BASE64_HPP
