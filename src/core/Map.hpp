#ifndef MAP_HPP
#define MAP_HPP

#include "Utils.hpp"

#include <string>
//#include <vector>

#include <sys/types.h>

namespace ore
{
    const unsigned int Max_Layes = 8;
    
    class Map
    {
    public:
        Map();
        ~Map();

        /**
         * This function loads a map from a .tmx file.
         */
        int Load(const std::string &file);

        /**
         * Gets the height of the map in tiles.
         */
        int GetHeight() const;

        /**
         * Gets the width of the map in tiles.
         */
        int GetWidth() const;

        /**
         * Gets the width of the tiles (in pixels of course).
         */
        int GetTileWidth() const;

        /**
         * Gets the height of the tiles (in pixels of course).
         */
        int GetTileHeight() const;
        
    private:
        /**
         * Path to the .tmx file.
         */
        std::string mFilePath;

        /**
         * Height of the map (in tiles).
         */
        uint mHeight;

        /**
         * Width of the map (in tiles).
         */
        uint mWidth;

        /**
         * Width of the tiles.
         */
        uint mTileWidth;

        /**
         * Height of the tiles.
         */
        uint mTileHeight;

        /**
         * A vector holding all tilesets of this map.
         */
        //std::vector<> mTilesets;

        /**
         * A two dimensional array of uints. The first dimension is the layer
         * the second is the data of the layer. Each tile is represented by a
         * unsigned int.
         */
        uint *mLayers[Max_Layes];
    };
}
#endif // MAP_HPP
