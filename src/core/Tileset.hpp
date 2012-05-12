#ifndef TILESET_HPP
#define TILESET_HPP

#include "Utils.hpp"

#include <string>

#include <SDL/SDL.h>

#include <sys/types.h>

namespace ore
{    
    class Tileset
    {
    public:
        Tileset();
        ~Tileset();

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
        int Load(const std::string &path, ore::uint fGid, ore::uint tileWidth = 0,
                 ore::uint tileHeight = 0);

        /**
         * This function reloads the tileset using mPath as the source image.
         * If mPath is not set or it fails to load the image mSurface becomes
         * NULL, and the data is reset to the defaults.
         * @return ore::SUCCESS or another ore::RETURN_VALUES.
         */
        int Reload();

        /**
         * Gets the path of the image.
         */
        const std::string& GetPath() const;
        
        /**
         * Gets the surface of this tileset.
         */
        SDL_Surface* GetSurface()
        {
            return mSurface;
        }
        
        /**
         * Gets the surface of this tileset.
         */
        const SDL_Surface* GetSurface() const
        {
            return mSurface;
        }
        
        /**
         * Blits a tile from this tileset into a given surface in the specified
         * point. Returns 0 on success or a RETURN_VALUES on failure.
         */
        int BlitGidToSurface(ore::uint gid,  SDL_Surface *destination,
                             ore::uint xpos = 0, ore::uint ypos = 0);
        
        /**
         * Checks if the given GID is part of this tileset.
         * @return true if is or false if it is not.
         */
        bool IsMyGid(ore::uint gid);
        
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
        ore::uint GetLastGid();
        
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
            return mSurface->h;
        }
        
        /**
         * Gets the width of the tileset image.
         * @return the tileset width in pixels.
         */
        ore::uint GetWidthP()
        {
            return mSurface->w;
        }

        /**
         * DO NOT USE THIS FUNCTION
         * This function is a workaround, it may lead to memory leaks if used
         * incorrectly. It will be removed ASAP.
         */
        void Clear();
        
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
         * Pointer to the SDL_Surface holding the content of this tileset.
         */
        SDL_Surface *mSurface;

        /**
         * The path to the image file.
         */
        std::string mPath;
    };
}
#endif // TILESET_HPP
