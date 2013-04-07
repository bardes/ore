#ifndef MAP_HPP
#define MAP_HPP

#include "Resource.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

namespace ore
{
    const unsigned int Max_Layers = 256;

    class Layer;
    class Tileset;

    class Map : public Resource
    {
    public:

        //TODO GetTilesets(), DeleteLayer, MoveLayer, GetLayerData
        //TODO mName
        Map();

        virtual ~Map();

        /**
         * @brief Gets the type of the resource;
         */
        virtual ore::ResourceType GetType() const
        {
            return ore::MAP_TYPE;
        }

        /**
         * @brief Loads a map from a .tmx file.
         */
        void Load(const std::string &file);   

        /**
         * @brief Renders the cache of all layers.
         */
        void RenderCache();

        /**
         * @brief Loads a new tileset and adds it to the map.
         * @param path Path to the tileset image.
         * @param fGid first GID of the tileset.
         * @param tw width of each tile (in pixels).
         * @param th height of each tile (in pixels).
         * @return The addres of the new tileset.
         */
        ore::Tileset* AddTileset(const std::string &path, ore::uint16 fGid,
                                 ore::uint8 tw, ore::uint8 th,
                                 ResourceManager* mgr = NULL);

        /**
         * @brief Adds the given tileset to the map.
         * @param newTileset address of the tileset.
         */
        void AddTileset(Tileset* newTileset, bool reg);

        /**
         * @brief Deletes a tileset from this map.
         * @param tileset Address of the tileset to be deleted.
         * @return True if can't find the tileset.
         */
        bool DeleteTileset(Tileset* tileset);

        sf::Sprite GetTilesetImg(ore::uint8)
        {
            return sf::Sprite();
        }

        /**
         * @brief Adds a new layer to the map.
         * 
         * By default it goes on top of the others, but this can be changed by
         * setting the pos value. Note that layers are always registerd on the
         * local resource manager.
         * @param data This should be the uncompressed data from a .tmx file.
         * @param width Width of the layer in tiles.
         * @param height Height of the layer in tiles.
         * @param name Name of the layer.
         * @param pos Position of the layer (0 is bottom). If pos is too big or
         * smaller than 0 the layer goes to the top.
         */
        ore::Layer* AddLayer(const std::string &data, const std::string &name, int pos = -1);

        /**
         * @brief Adds a layer to the map.
         * 
         * By default it goes on top of the others, but this can be changed by
         * setting the pos value. Note that layers are always registerd on the
         * local resource manager.
         * @param newLayer address of the layer to be added.
         * @param pos Position of the layer (0 is bottom). If pos is too big or
         * smaller than 0 the layer goes to the top.
         */
        void AddLayer(Layer* newLayer, int pos = -1);

        /**
         * @brief Gets the image of a layer.
         * @param layer Number of the layer. (0 is the bottom most layer)
         * @return A sprite with the image.
         */
        sf::Sprite GetLayerImg(ore::uint16 layer);

        /**
         * @brief Gets number of layers of this map.
         */
        ore::uint8 GetLayersN() const
        {
            return mLayers.size();
        }

        /**
         * @brief Clears all data from the map.
         */
        void Clear();

        /**
         * @brief Gets the height of the map in tiles.
         */
        ore::uint8 GetHeight() const
        {
            return mHeight;
        }

        /**
         * @brief Gets the width of the map in tiles.
         */
        ore::uint8 GetWidth() const
        {
            return mWidth;
        }

        /**
         * @brief Gets the height of the tiles (in pixels of course).
         */
        ore::uint8 GetTileHeight() const
        {
            return mTileHeight;
        }

        /**
         * @brief Gets the width of the tiles (in pixels of course).
         */
        ore::uint8 GetTileWidth() const
        {
            return mTileWidth;
        }

    private:
        /**
         * @brief Path to the .tmx file.
         */
        std::string mFilePath;

        /**
         * @brief Width of the map (in tiles).
         */
        ore::uint8 mWidth;

        /**
         * @brief Height of the map (in tiles).
         */
        ore::uint8 mHeight;

        /**
         * @brief Width of the tiles.
         */
        ore::uint8 mTileWidth;

        /**
         * @brief Height of the tiles.
         */
        ore::uint8 mTileHeight;

        /**
         * @brief A vector of pointers to the tilesets of this map.
         */
        std::vector<Tileset*> mTilesets;

        /**
         * @brief A vector of pointers to the layers of this map.
         */
        std::vector<Layer*> mLayers;

        /**
         * @brief Default local resource manager.
         */
        ResourceManager mLocalMgr;
    };
}
#endif // MAP_HPP
