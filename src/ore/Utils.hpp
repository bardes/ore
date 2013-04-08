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
    enum Exception
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
    enum ResourceType
    {
        MAP_TYPE,
        TILESET_TYPE,
        LAYER_TYPE,
        IMAGE_TYPE,
        OBJECT_TYPE
    };

    /**
     * Prop types used to perform type checking when necessary.
     */
    enum PropType
    {
        INT_PROP,
        FLOAT_PROP,
        STRING_PROP,
        RAW_DATA_PROP,
        NEW_PROP,
        UNKNOWN_PROP
    };

    /**
     * Possible orientation values.
     */
    enum Orientation
    {
        NORTH = 0x000001,
        EAST =  0x000002,
        SOUTH = 0x000004,
        WEST =  0x000008
    };

    //Structs
    
    /**
     * This struct is a way to store different kinds of information using the
     * same object.
     */
    struct Property
    {
        /**
         * Type of the value stored in this prop.
         */
        PropType type;
        
        /**
         * Here is where the magic happens and the value is stored in a different 
         * variable according to it's type.
         */
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
        
        ~Property()
        {
            if(type == STRING_PROP)
                delete str;
            else if(type == RAW_DATA_PROP)
                delete raw;
        }
        
    };

    //TODO Put these in a separate file...
    //TODO Compress()
    int Uncompress(const std::vector<ore::uint8>& data, std::vector<ore::uint8>& result,
                   ore::uint32 expectedSize);
}
#endif // UTILS_HPP
