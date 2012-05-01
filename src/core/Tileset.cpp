#include "Tileset.hpp"
#include "Utils.hpp"

#include <iostream>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <sys/types.h>

ore::Tileset::Tileset() : mFirstGid(-1), mTileHeight(0), mTileWidth(0),
mSurface(NULL), mPath("")
{
    //Nothing to do here.
}

ore::Tileset::~Tileset()
{
    SDL_FreeSurface(mSurface);
}

int ore::Tileset::Load (const std::string &path, int fGid, uint tileWidth,
                          uint tileHeight)
{
    //Checking if SDL wasn't initialized yet
    if(!SDL_WasInit(SDL_INIT_VIDEO))
        return ERR_NO_SDL_VIDEO;
    
    //Freeing the current surface, And getting rid of the old data
    SDL_FreeSurface(mSurface);
    mSurface = NULL;
    mTileHeight = 0;
    mTileWidth = 0;
    mFirstGid = -1;
    mPath.clear();
    
    //Temporary surface to load the image
    SDL_Surface *tmp = NULL;
    
    //Loading
    if((IMG_Load(path.c_str())) == NULL)
    {
        std::cerr << "SDL error: " << SDL_GetError();
        return ERR_FILE_READ_ERROR;
    }
    
    //Trying to optimize the surface
    if((mSurface = SDL_DisplayFormat(tmp)) == NULL)
    {
        SDL_FreeSurface(tmp);
        std::cerr << "SDL error: " << SDL_GetError();
        return ERR_UNKNOWN;
    }
    SDL_FreeSurface(tmp);

    //Copying the loaded data into the tileset
    mPath = path;
    mFirstGid = fGid;
    mTileHeight = tileHeight;
    mTileWidth = tileWidth;
}

int ore::Tileset::Reload()
{
    if(mPath.empty())
        return ERR_NO_PATH;
    
    return Load(mPath, mFirstGid, mTileWidth, mTileHeight);
    
}

