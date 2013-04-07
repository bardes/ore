#ifndef TILESET_HPP
#define TILESET_HPP

#include "Image.hpp"
#include "Resource.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"

#include <SFML/Graphics.hpp>

#include <string>

#include <sys/types.h>

namespace ore
{
    class Tileset : public Resource
    {
    public:
        /**
         * Default constructor.
         */
        Tileset();
        virtual ~Tileset();

        /**
         * @brief Gets the type of the resource;
         */
        virtual ore::ResourceType GetType() const
        {
            return ore::TILESET_TYPE;
        }

        /**
         * @brief Loads the tileset from a source image.
         * @param path relative or absolute path to the image file.
         * @param fGid the first gid of this tileset.
         * @param tileWidth width of the tiles in pixels.
         * @param tileHeight height of the tiles in pixels.
         * @param mgr Resource manager to keep track of the tileset image.
         * @return false if failed to load.
         */
        bool Load(const std::string &path, ore::uint16 fGid, ore::uint8 tileWidth,
                  ore::uint8 tileHeight, ore::ResourceManager *mgr = NULL);

        /**
         * This function reloads the tileset, leaving it unchanged in case of failure.
         * @return Returns false if it fails to load the image.
         */
        bool Reload();

        /**
         * Checks if the given GID is part of this tileset.
         *
         * @param gid Global ID of the tile you want to check.
         * @return true if is or false if it is not.
         */
        bool IsMyGid(ore::uint16 gid)
        {
            return (gid >= mFirstGid) && (gid <= mLastGid);
        }

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
            return mImage->GetTexture();
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
            return (mHeight * mWidth) + mFirstGid - 1;
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
            return mImage->GetTexture().getSize().y;
        }

        /**
         * Gets the width of the tileset image.
         * @return the tileset width in pixels.
         */
        ore::uint32 GetWidthP()
        {
            return mImage->GetTexture().getSize().x;
        }

        sf::Sprite GetGidImg(ore::uint16 gid);

        void Clear();

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
        ore::Image* mImage;

        /**
         * The path to the image file.
         */
        std::string mPath;

        ResourceManager mLocalMgr;
    };
}
#endif // TILESET_HPP
