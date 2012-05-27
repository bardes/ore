#ifndef MAP_HPP
#define MAP_HPP

#include "Tileset.hpp"
#include "Utils.hpp"

#include <list>
#include <string>

#include <SFML/Graphics.hpp>

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
         * @return ore::SUCCESS or another ore::RETURN_VALUES in case of failure.
         */
        ore::RETURN_VALUE Load(const std::string &file);
        
        /**
         * This function renders a layer of this map into the given sf::Drawable
         */
        ore::RETURN_VALUE RenderLayer(ore::uint layer,
                                      const sf::Drawable &destination);

        /**
         * Gets the height of the map in tiles.
         */
        ore::uint GetHeight() const
        {
            return mHeight;
        }

        /**
         * Gets the width of the map in tiles.
         */
        ore::uint GetWidth() const
        {
            return mWidth;
        }

        /**
         * Gets the height of the tiles (in pixels of course).
         */
        ore::uint GetTileHeight() const
        {
            return mTileHeight;
        }
        
        /**
         * Gets the width of the tiles (in pixels of course).
         */
        ore::uint GetTileWidth() const
        {
            return mTileWidth;
        }

        /**
         * Sets the layer in which the player should be drawn on this map.
         */
        void SetPlayerLayer(ore::uint layer)
        {
            if(layer <= mLayerCount)
                mPlayerLayer = layer;
        }
        
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
         * Number of layers of this map.
         */
        ore::uint mLayerCount;
        
        /**
         * The layer in which the player will be drawn over
         */
        ore::uint mPlayerLayer;

        /**
         * A list with all tilesets of this map.
         */
        std::list<Tileset*> mTilesets;

        /**
         * A two dimensional array of ore::uints. The first dimension is the layer
         * the second is the data of the layer. Each tile is represented by a
         * GID, which is an unsigned int.
         */
        //TODO Make a Layer class and use a std::list here.
        ore::uint *mLayers[Max_Layers];
        
        /**
         * This holds a pre-rendered image of all bottom layers (under the player)
         */
        sf::Texture mLayerCacheBotom;
        
        /**
         * This holds a pre-rendered image of all top layers (above the player)
         */
        sf::Texture mLayerCachetop;
    };
}
#endif // MAP_HPP
