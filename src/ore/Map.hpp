#ifndef MAP_HPP
#define MAP_HPP

#include "Layer.hpp"
#include "Tileset.hpp"
#include "Utils.hpp"

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

namespace ore
{
    const unsigned int Max_Layers = 256;

    class Map
    {
    public:

        Map();

        ~Map();

        /**
         * This function loads a map from a .tmx file.
         */
        void Load(const std::string &file);

        /**
         * This functions adds a tileset to the map.
         * @param path Path to the tileset image.
         * @param fGid first GID of the tileset.
         * @param tw width of each tile (in pixels).
         * @param th height of each tile (in pixels).
         */
        void AddTileset(std::string &path, ore::uint16 fGid,
                                     ore::uint8 tw, ore::uint8 th);

        /***
         * This function deletes a tileset from this map. The first gid of the
         * tileset is used to idntify which tileset will be deleted.
         * @return false if can't find or delete the tileset.
         */
        //TODO bool DeleteTileset(sf::uint16 firstGid)

        /**
         * This function adds a layer on top of the others.
         * @param data This should be the uncompressed data from a .tmx file.
         * @param width Width of the layer in tiles.
         * @param height Height of the layer in tiles.
         */
        void AddLayer(const std::string &data, ore::uint8 width,
                      ore::uint8 height, const std::string &name);

        /**
         * Clears all data from the map. It does delete the layers and tilesets
         * also.
         */
        void Clear();

        /**
         * Gets the height of the map in tiles.
         */
        ore::uint8 GetHeight() const
        {
            return mHeight;
        }

        /**
         * Gets the width of the map in tiles.
         */
        ore::uint8 GetWidth() const
        {
            return mWidth;
        }

        /**
         * Gets the height of the tiles (in pixels of course).
         */
        ore::uint8 GetTileHeight() const
        {
            return mTileHeight;
        }

        /**
         * Gets the width of the tiles (in pixels of course).
         */
        ore::uint8 GetTileWidth() const
        {
            return mTileWidth;
        }

        /**
         * Sets the layer in which the player should be drawn on this map.
         */
        void SetPlayerLayer(ore::uint8 layer)
        {
            if(layer <= mLayers.size())
                mPlayerLayer = layer;
        }

        // Just for tests (remove later)
        ore::uint8 GetLayers() const
        {
            return mLayers.size();
        }

    private:
        /**
         * Path to the .tmx file.
         */
        std::string mFilePath;

        /**
         * Width of the map (in tiles).
         */
        ore::uint8 mWidth;

        /**
         * Height of the map (in tiles).
         */
        ore::uint8 mHeight;

        /**
         * Width of the tiles.
         */
        ore::uint8 mTileWidth;

        /**
         * Height of the tiles.
         */
        ore::uint8 mTileHeight;

        /**
         * The layer in which the player will be drawn over
         */
        ore::uint8 mPlayerLayer;

        /**
         * Flag to indicate whether the cache needs to be updated or not.
         */
        bool mCached;

        /**
         * A vector of pointers to the tilesets of this map.
         */
        std::vector<Tileset*> mTilesets;

        /**
         * A vector of pointers to the layers of this map.
         */
        std::vector<Layer*> mLayers;

        /**
         * This holds a pre-rendered image of all bottom layers (under the player)
         */
        sf::RenderTexture mLayerCacheBotom;

        /**
         * This holds a pre-rendered image of all top layers (above the player)
         */
        sf::RenderTexture mLayerCacheTop;
    };
}
#endif // MAP_HPP
