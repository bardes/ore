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
        Tileset();
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
        ore::RETURN_VALUE Load(const std::string &path, ore::uint fGid,
                               ore::uint tileWidth, ore::uint tileHeight);

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
        bool IsMyGid(ore::uint gid);

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
        ore::uint GetFirstGid()
        {
            return mFirstGid;
        }
        
        /**
         * Gets the last GID belonging to this tileset.
         * @return the GID of the last tile.
         */
        ore::uint GetLastGid()
        {
            return mHeight * mWidth + mFirstGid - 1;
        }
        
        /**
         * Gets the tile height.
         * @return tile height in pixels.
         */
        ore::uint GetTileHeight()
        {
            return mTileHeight;
        }
        
        /**
         * Gets the tile width.
         * @return tile width in pixels.
         */
        ore::uint GetTileWidth()
        {
            return mTileWidth;
        }
        
        /**
         * Gets the height of the tileset image.
         * @return the tileset height in pixels.
         */
        ore::uint GetHeightP()
        {
            return mTexture.getSize().y;
        }
        
        /**
         * Gets the width of the tileset image.
         * @return the tileset width in pixels.
         */
        ore::uint GetWidthP()
        {
            return mTexture.getSize().x;
        }
        
    private:
        /**
         * First GID represented by this tile. -1 if not set.
         * (GID = Global ID = A number that represents any tile from any tiliset) 
         */
        ore::uint mFirstGid;
        
        /**
         * Last GID in this tileset.
         */
        ore::uint mLastGid;
        
        /**
         * Tile height.
         */
        ore::uint mTileHeight;
        
        /**
         * Tile width.
         */
        ore::uint mTileWidth;
        
        /**
         * Height in tiles.
         */
        ore::uint mHeight;
        
        /**
         * Width in tiles.
         */
        ore::uint mWidth;

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
