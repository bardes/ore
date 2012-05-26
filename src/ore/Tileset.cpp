#include "Tileset.hpp"
#include "Utils.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>

#include <sys/types.h>

ore::Tileset::Tileset() : mFirstGid(0), mLastGid(0), mTileHeight(0),mTileWidth(0),
mHeight(0), mWidth(0), mPath("")
{
    //Nothing to do here.
}

int ore::Tileset::Load (const std::string& path, ore::uint fGid,
                        ore::uint tileWidth, uint tileHeight)
{
    //Getting rid of the old data
    mFirstGid = mLastGid = mTileHeight = mTileWidth = mHeight = mWidth = 0;
    mPath.clear();
    
    //Loading the tiles from the file.
    if(!mTexture.loadFromFile(path))
    {
        std::cerr << "SFML error: Dunno what happened...";
        return ore::ERR_UNKNOWN;
    }

    //Saving the loaded data into the tileset
    mPath = path;
    mFirstGid = fGid;
    mTileHeight = tileHeight;
    mTileWidth = tileWidth;
    mHeight = mTexture.getSize().y / mTileHeight;
    mWidth = mTexture.getSize().x / tileWidth;
    
    return ore::SUCCESS;
}

int ore::Tileset::Reload()
{
    if(mPath.empty())
        return ore::ERR_NO_PATH;
    
    return Load(mPath, mFirstGid, mTileWidth, mTileHeight);
    
}

bool ore::Tileset::IsMyGid(ore::uint gid)
{
    return (gid >= mFirstGid) && (gid <= GetLastGid());
}