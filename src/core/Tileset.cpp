#include "Tileset.hpp"
#include "Utils.hpp"

#include <iostream>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <sys/types.h>

ore::Tileset::Tileset() : mFirstGid(0), mTileHeight(0), mTileWidth(0),
mSurface(NULL), mPath("")
{
    //Nothing to do here.
}

ore::Tileset::~Tileset()
{
    if(mSurface != NULL)
        SDL_FreeSurface(mSurface);
}

int ore::Tileset::Load (const std::string& path, ore::uint fGid, ore::uint tileWidth,
                        uint tileHeight)
{
    //Checking if SDL wasn't initialized yet
    if(!SDL_WasInit(SDL_INIT_VIDEO))
        return ERR_NO_SDL_VIDEO;
    
    //Freeing the current surface, And getting rid of the old data
    SDL_FreeSurface(mSurface);
    mSurface = NULL;
    mFirstGid = mTileHeight = mTileWidth = 0;
    mPath.clear();
    
    //Temporary surface to load the image
    SDL_Surface *tmp = NULL;
    
    //Loading
    if((tmp = IMG_Load(path.c_str())) == NULL)
    {
        std::cerr << "SDL error: " << SDL_GetError();
        return ERR_IMAGE_READ_ERROR;
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
    return SUCCESS;
}

int ore::Tileset::Reload()
{
    if(mPath.empty())
        return ERR_NO_PATH;
    
    return Load(mPath, mFirstGid);
    
}

const std::string& ore::Tileset::GetPath() const
{
    return mPath;
}

ore::uint ore::Tileset::GetLastGid()
{
    return (mSurface->h / mTileHeight)  *
    (mSurface->w / mTileWidth) + mFirstGid - 1;
}

bool ore::Tileset::IsMyGid(ore::uint gid)
{
    return (gid >= mFirstGid) && (gid <= GetLastGid());
}

void ore::Tileset::Clear()
{
    mSurface = NULL;
    mFirstGid = mTileHeight = mTileWidth = 0;
    mPath.clear();
}