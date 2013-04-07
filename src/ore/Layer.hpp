#ifndef LAYER_HPP
#define LAYER_HPP

#include "Resource.hpp"
#include "Tileset.hpp"
#include "Utils.hpp"

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>

namespace ore
{
    class Layer : public Resource
    {
    public:
        //TODO Default constructor
        //Funções de acesso.

        Layer(ore::uint8 w, ore::uint8 h, uint8 tw, uint8 th, const std::string &name);

        virtual ore::ResourceType GetType() const
        {
            return ore::LAYER_TYPE;
        }

        /**
         * Resets all data to default.
         */
        void Clear();

        /**
         * @brief Renders this layer and stores the result in cache.
         * @return The rendered image.
         */
        sf::Sprite RenderCache(const std::vector<Tileset*>& tilesets);

        sf::Sprite GetImage() const
        {
            return sf::Sprite(mCache.getTexture());
        }

        /**
         * Gets the height of the layer in tiles.
         */
        ore::uint8 GetHeight() const
        {
            return mHeight;
        }

        /**
         * Gets the width of the layer in tiles.
         */
        ore::uint8 GetWidth() const
        {
            return mWidth;
        }

        /**
         * Gets the index of a tile based on it's position
         */
        ore::uint16 GetIndex(ore::uint8 xPos, ore::uint8 yPos)
        {
            return (yPos * mWidth) + xPos;
        }

        /**
         * Gets a GID based on it's position.
         * @return the GID value, or 0 in case of failure.
         */
        ore::uint16 GetGid(ore::uint8 xPos, ore::uint8 yPos) const
        {
            if((yPos * mWidth) + xPos >= mGids.size())
                return 0;

            return mGids[(yPos * mWidth) + xPos];
        }

        /**
         * Gets a GID by the index.
         * @return the GID value, or 0 in case of failure.
         */
        ore::uint16 GetGid(ore::uint16 index) const
        {
            if(index >= mGids.size())
                return 0;

            return mGids[index];
        }

        void AddGid(ore::uint16 gid)
        {
            mGids.push_back(gid);
        }

        ore::uint8 GetTileWidth() const
        {
            return mTileWidth;
        }

        ore::uint8 GetTileHeight() const
        {
            return mTileHeight;
        }
        //TODO SetData();

    private:
        /**
         * Width of the layer in tiles.
         */
        ore::uint8 mWidth;

        /**
         * Height of the layer in tiles.
         */
        ore::uint8 mHeight;

        /**
         * Width of the "regular" tiles.
         */
        ore::uint8 mTileWidth;

        /**
         * Hight of the "regular" tiles.
         */
        ore::uint8 mTileHeight;

        /**
         * Name of the layer.
         */
        std::string mName;

        /**
         * This holds the GIDs of this layer.
         */
        std::vector<ore::uint16> mGids;

        /**
         * Cached renderd image of this layer.
         */
        sf::RenderTexture mCache;
    };
}
#endif // LAYER_HPP
