#ifndef TILESET_HPP
#define TILESET_HPP

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
         * @param path relative or absolute path to the image file
         * @param fGid the first gid of this tileset
         * @param tileWidth width of the tiles in pixels
         * @param tileHeight height of the tiles in pixels
         * @return Returns 0 on success or another [RETURN_VALUE](@ref ore::RETURN_VALUE)
         * in case of failure.
         */
        int Load(const std::string &path, uint fGid);

        /**
         * This function reloads the tileset using mPath as the source image.
         * If mPath is not set or it fails to load the image mSurface becomes
         * NULL, and the data is reset to the defaults.
         */
        int Reload();

        /**
         * Gets the path of the image.
         */
        const std::string& GetPath() const;

        /**
         * DO NOT USE THIS FUNCTION
         * This function is a workaround, it may lead to memory leaks if used
         * incorrectly. It will be removed ASAP.
         */
        void Clear();
        
    private:
        /**
         * First id represented by this tile. -1 if not set.
         * (gid = Global ID) 
         */
        uint mFirstGid;

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
