#ifndef MAP_HPP
#define MAP_HPP

#include "Tileset.hpp"
#include "Utils.hpp"

#include <string>
#include <list>

#include <sys/types.h>

namespace ore
{
    const unsigned int Max_Layers = 8;
    
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
        ore::uint mHeight;

        /**
         * Width of the map (in tiles).
         */
        ore::uint mWidth;

        /**
         * Width of the tiles.
         */
        ore::uint mTileWidth;

        /**
         * Height of the tiles.
         */
        ore::uint mTileHeight;

        /**
         * A list holding all tilesets of this map.
         */
        std::list<Tileset> mTilesets;

        /**
         * A two dimensional array of ore::uints. The first dimension is the layer
         * the second is the data of the layer. Each tile is represented by a
         * GID, which is an unsigned int .
         */
        ore::uint *mLayers[Max_Layers];
    };
}
#endif // MAP_HPP
