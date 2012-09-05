#ifndef LAYER_HPP
#define LAYER_HPP

#include "Utils.hpp"

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>

namespace ore
{
    class Layer
    {
    public:

        Layer(ore::uint8 w, ore::uint8 h, const std::string &name);

        ore::uint16& operator[](ore::uint16 index);

        const ore::uint16& operator[](ore::uint16 index) const;

        /**
         * Resets all data to default.
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
            return mGids[(yPos * mWidth) + xPos];
        }

        /**
         * Gets a GID by the index.
         * @return the GID value, or 0 in case of failure.
         */
        ore::uint16 GetGid(ore::uint16 index) const
        {
            return mGids[index];
        }

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
         * Name of the layer.
         */
        std::string mName;

        /**
         * This holds the GIDs of this layer.
         */
        std::vector<ore::uint16> mGids;
    };
}
#endif // LAYER_HPP
