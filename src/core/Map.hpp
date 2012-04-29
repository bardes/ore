#ifndef MAP_HPP
#define MAP_HPP

#include <list>
#include <string>
#include <map>

namespace ore
{
    enum MAP_ERRORS {
        ERR_FILE_READ_ERROR = 1,
        ERR_INVALID_MAP_DATA,
        ERR_INVALID_TILE_DATA,
        ERR_ZLIB_ERROR,
        ERR_INVALID_COMPRESSION_METHOD,
        ERR_INVALID_TILESET
    };
    
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
        int AddTileset(const char *tilesetPath);
        
    private:
        std::string mFilePath;
        int mHeight;
        int mWidth;
        int mTileWidth;
        int mTileHeight;
        unsigned char mTilesetCnt;
        
        int *mTiles;
    };
}
#endif // MAP_HPP
