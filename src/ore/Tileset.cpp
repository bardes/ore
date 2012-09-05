#include "Tileset.hpp"
#include "Utils.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>

#include <sys/types.h>

ore::Tileset::Tileset() : mFirstGid(0), mLastGid(0), mTileHeight(0),
mTileWidth(0), mHeight(0), mWidth(0), mPath("")
{
    //Nothing to do here.
}

void ore::Tileset::Load (const std::string& path, ore::uint16 fGid,
                                      ore::uint8 tileWidth, uint8 tileHeight)
{
    //Getting rid of the old data
    mFirstGid = mLastGid = mTileHeight = mTileWidth = mHeight = mWidth = 0;
    mPath.clear();

    //Loading the tiles from the file.
    if(!mTexture.loadFromFile(path))
    {
        std::cerr << "Error: could not load this image: " << path << ".\n";
        throw ore::IMAGE_READ_ERROR;
    }

    //Saving the loaded data into the tileset
    mPath = path;
    mFirstGid = fGid;
    mTileHeight = tileHeight;
    mTileWidth = tileWidth;
    mHeight = mTexture.getSize().y / mTileHeight;
    mWidth = mTexture.getSize().x / tileWidth;
}

void ore::Tileset::Reload()
{
    if(mPath.empty())
        throw ore::NO_PATH;

    Load(mPath, mFirstGid, mTileWidth, mTileHeight);

}

bool ore::Tileset::IsMyGid(ore::uint16 gid)
{
    return (gid >= mFirstGid) && (gid <= mLastGid);
}
