#ifndef LAYER_HPP
#define LAYER_HPP

#include "Utils.hpp"

#include <vector>

namespace ore
{
    class Layer
    {
    public:

        Layer(ore::uint8 width, ore::uint8 height);

        //Layer(const Layer& other);

        ore::uint16& operator[](ore::uint16 index);

        const ore::uint16& operator[](ore::uint16 index) const;

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
         * Gets a GID based in it's position.
         * @return the GID value, or 0 in case of failure.
         */
        ore::uint16 GetGid(ore::uint8 xPos, ore::uint8 yPos) const;

        /**
         * Gets a GID by the index.
         * @return the GID value, or 0 in case of failure.
         */
        ore::uint16 GetGid(ore::uint16 index) const;

        /**
         * This function draws this layer into the given drawable object, and
         * also draws the given characters if necessary.
         */
        //ore::RETURN_VALUE Draw() const; //TODO


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
         * Width of the tiles.
         */
        ore::uint8 mTileWidth;

        /**
         * Height of the tiles.
         */
        ore::uint8 mTileHeight;

        /**
         * This holds the GIDs of this layer.
         */
        std::vector<ore::uint16> mGids;
    };
}
#endif // LAYER_HPP
