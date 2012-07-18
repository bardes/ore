#ifndef TILESET_HPP
#define TILESET_HPP

#include "Utils.hpp"

#include <SFML/Graphics.hpp>

#include <string>

#include <sys/types.h>

namespace ore
{
    class Tileset
    {
    public:
        /**
         * Default constructor.
         */
        Tileset();

        //TODO Other kinds of constructors

        //~Tileset();

        /**
         * This function loads the tileset from a source image. If it fails to
         * load the image mSurface becomes NULL, and all the data is reset to
         * the defaults.
         *
         * @param path relative or absolute path to the image file.
         * @param fGid the first gid of this tileset.
         * @param tileWidth width of the tiles in pixels.
         * @param tileHeight height of the tiles in pixels.
         * @return ore::SUCCESS on success or another ore::RETURN_VALUES in case
         * of failure in case of failure.
         */
        ore::RETURN_VALUE Load(const std::string &path, ore::uint16 fGid,
                               ore::uint8 tileWidth, ore::uint8 tileHeight);

        /**
         * This function reloads the tileset using mPath as the source image.
         * If mPath is not set or it fails to load the image mSurface becomes
         * NULL, and the data is reset to the defaults.
         * @return ore::SUCCESS or another ore::RETURN_VALUES.
         */
        ore::RETURN_VALUE Reload();

        /**
         * Checks if the given GID is part of this tileset.
         *
         * @param gid Global ID of the tile you want to check.
         * @return true if is or false if it is not.
         */
        bool IsMyGid(ore::uint16 gid);

        /**
         * Gets the path of the image.
         */
        const std::string& GetPath() const
        {
            return mPath;
        }

        /**
         * Gets the surface of this tileset.
         */
        const sf::Texture& GetTexture() const
        {
            return mTexture;
        }

        /**
         * Gets the first GID belonging to this tiliset.
         * @return the GID of the first tile.
         */
        ore::uint16 GetFirstGid()
        {
            return mFirstGid;
        }

        /**
         * Gets the last GID belonging to this tileset.
         * @return the GID of the last tile.
         */
        ore::uint16 GetLastGid()
        {
            return mHeight * mWidth + mFirstGid - 1;
        }

        /**
         * Gets the tile height.
         * @return tile height in pixels.
         */
        ore::uint8 GetTileHeight()
        {
            return mTileHeight;
        }

        /**
         * Gets the tile width.
         * @return tile width in pixels.
         */
        ore::uint8 GetTileWidth()
        {
            return mTileWidth;
        }

        /**
         * Gets the height of the tileset image.
         * @return the tileset height in pixels.
         */
        ore::uint32 GetHeightP()
        {
            return mTexture.getSize().y;
        }

        /**
         * Gets the width of the tileset image.
         * @return the tileset width in pixels.
         */
        ore::uint32 GetWidthP()
        {
            return mTexture.getSize().x;
        }

    private:
        /**
         * First GID represented by this tile. -1 if not set.
         * (GID = Global ID = A number that represents any tile from any tiliset)
         */
        ore::uint16 mFirstGid;

        /**
         * Last GID in this tileset.
         */
        ore::uint16 mLastGid;

        /**
         * Tile height.
         */
        ore::uint8 mTileHeight;

        /**
         * Tile width.
         */
        ore::uint8 mTileWidth;

        /**
         * Height in tiles.
         */
        ore::uint8 mHeight;

        /**
         * Width in tiles.
         */
        ore::uint8 mWidth;

        /**
         * This holds the tiles image.
         */
        sf::Texture mTexture;

        /**
         * The path to the image file.
         */
        std::string mPath;
    };
}
#endif // TILESET_HPP
