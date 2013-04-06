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
    
//     typedef boost::variant<int, float, std::string, std::vector< ore::uint8 > > property;

    //Enums
    /**
     * Values to be thrown during exceptions.
     */
    enum EXCEPTION
    {
        BAD_ALLOC,
        INDEX_OUT_OF_RANGE,
        INVALID_COMPRESSION_METHOD,
        INVALID_MAP_DATA,
        INVALID_PROP_CONVERSION,
        INVALID_TILESET,
        IMAGE_READ_ERROR,
        NO_PATH,
        MAP_READ_ERROR,
        PROP_NOT_FOUND,
        TILESET_COLLISION,
        ZLIB_ERROR
    };

    /**
     * Resource types available.
     */
    enum RESOURCE_TYPE
    {
        MAP_TYPE,
        TILESET_TYPE,
        LAYER_TYPE,
        IMAGE_TYPE,
        OBJECT_TYPE
    };

    enum PROP_TYPE
    {
        INT_PROP,
        FLOAT_PROP,
        STRING_PROP,
        RAW_DATA_PROP,
        NEW_PROP,
        UNKNOWN_PROP
    };

    enum ORIENTATION
    {
        NORTH = 0x000001,
        EAST =  0x000002,
        SOUTH = 0x000004,
        WEST =  0x000008
    };

    //Structs
    struct Property
    {
        PROP_TYPE type;
        
        union
        {
            int i;
            float f;
            std::string* str;
            std::vector< ore::uint8 >* raw;
        };
        
        Property() : type(NEW_PROP)
        {
            i = 0;
            f = 0;
            str = 0;
            raw = 0;
            
        }
        Property(int n) : type(INT_PROP), i(n)
        {
            //Yay!
        }
        Property(float n) : type(FLOAT_PROP), f(n)
        {
            //Yay!
        }
        Property(const std::string& s) : type(STRING_PROP)
        {
            str = new std::string(s);
        }
        Property(const std::vector< ore::uint8 >& r) : type(RAW_DATA_PROP)
        {
            raw = new std::vector< ore::uint8 >(r);
        }
    };

    //TODO Colocar num arquivo separado
    //TODO Compress()
    int Uncompress(const std::vector<ore::uint8>& data, std::vector<ore::uint8>& result,
                   ore::uint32 expectedSize);
}
#endif // UTILS_HPP
