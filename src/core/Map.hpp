#ifndef MAP_HPP
#define MAP_HPP

#include <list>
#include <string>
#include <map>
#include <sys/types.h>
#include <vector>

namespace ore
{
    //Constants
    const unsigned int Max_Layes = 8;

    //Enums
    enum MAP_ERRORS {
        ERR_FILE_READ_ERROR = 1,
        ERR_INVALID_MAP_DATA,
        ERR_INVALID_TILE_DATA,
        ERR_ZLIB_ERROR,
        ERR_INVALID_COMPRESSION_METHOD,
        ERR_INVALID_TILESET
    };

    //Classes
    class Map
    {
    public:
        Map();
        ~Map();
        int Load(const char *file);
        int GetHeight() const;
        int GetWidth() const;
        int GetTileWidth() const;
        int GetTileHeight() const;
        
    private:
        std::string mFilePath;
        uint mHeight;
        uint mWidth;
        uint mTileWidth;
        uint mTileHeight;
        std::vector<std::string> mTileSetsPath;
        uint *mLayers[Max_Layes];
    };
}
#endif // MAP_HPP
