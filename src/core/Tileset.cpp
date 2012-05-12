#include "Tileset.hpp"
#include "Utils.hpp"

#include <iostream>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <sys/types.h>

ore::Tileset::Tileset() : mFirstGid(0), mLastGid(0), mTileHeight(0),mTileWidth(0),
mHeight(0), mWidth(0), mSurface(NULL), mPath("")
{
    //Nothing to do here.
}

ore::Tileset::~Tileset()
{
    if(mSurface != NULL)
        SDL_FreeSurface(mSurface);
}

int ore::Tileset::Load (const std::string& path, ore::uint fGid,
                        ore::uint tileWidth, uint tileHeight)
{
    //Checking if SDL wasn't initialized yet
    if(!SDL_WasInit(SDL_INIT_VIDEO))
        return ore::ERR_NO_SDL_VIDEO;
    
    //Freeing the current surface, And getting rid of the old data
    SDL_FreeSurface(mSurface);
    mSurface = NULL;
    mFirstGid = mLastGid = mTileHeight = mTileWidth = mHeight = mWidth = 0;
    mPath.clear();
    
    //Temporary surface to load the image
    SDL_Surface *tmp = NULL;
    
    //Loading
    if((tmp = IMG_Load(path.c_str())) == NULL)
    {
        std::cerr << "SDL error: " << SDL_GetError();
        return ore::ERR_IMAGE_READ_ERROR;
    }
    
    //Trying to optimize the surface
    if((mSurface = SDL_DisplayFormat(tmp)) == NULL)
    {
        SDL_FreeSurface(tmp);
        std::cerr << "SDL error: " << SDL_GetError();
        return ore::ERR_UNKNOWN;
    }
    SDL_FreeSurface(tmp);

    //Saving the loaded data into the tileset
    mPath = path;
    mFirstGid = fGid;
    mTileHeight = tileHeight;
    mTileWidth = tileWidth;
    mHeight = mSurface->h / tileHeight;
    mWidth = mSurface->w / tileWidth;
    mLastGid = (mSurface->h / mTileHeight) *
               (mSurface->w / mTileWidth) + mFirstGid - 1;
    return ore::SUCCESS;
}

int ore::Tileset::Reload()
{
    if(mPath.empty())
        return ore::ERR_NO_PATH;
    
    return Load(mPath, mFirstGid);
    
}

//TODO inline
const std::string& ore::Tileset::GetPath() const
{
    return mPath;
}

ore::uint ore::Tileset::GetLastGid()
{
    return mLastGid;
}

bool ore::Tileset::IsMyGid(ore::uint gid)
{
    return (gid >= mFirstGid) && (gid <= GetLastGid());
}

int ore::Tileset::BlitGidToSurface(ore::uint gid, SDL_Surface* destination,
                                   ore::uint xpos, ore::uint ypos)
{
    if(mSurface == NULL)
        return ore::ERR_TILESET_NO_SURFACE;
    
    if(!IsMyGid(gid))
        return ore::ERR_TILESET_GID_OUT_OF_RANGE;
    
    int gidPosX, gidPosY;
    gidPosX = (gid % mWidth) * mTileWidth;
    gidPosY = (gid / mWidth) * mTileHeight;
    
    SDL_Rect srcRect, destRect;
    srcRect.w = mTileWidth;
    srcRect.h = mTileHeight;
    srcRect.x = gidPosX;
    srcRect.y = gidPosY;
    destRect.x = xpos;
    destRect.y = ypos;
    
    //TODO check for errors
    SDL_BlitSurface(mSurface, &srcRect, destination, &destRect);
    
    return ore::SUCCESS;
}


void ore::Tileset::Clear()
{
    mSurface = NULL;
    mFirstGid = mLastGid = mTileHeight = mTileWidth = mHeight = mWidth = 0;
    mPath.clear();
}