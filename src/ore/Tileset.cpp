#include "Tileset.hpp"

#include "Image.hpp"

#include <iostream>

#include <sys/types.h>

ore::Tileset::Tileset() : mFirstGid(0), mLastGid(0), mTileHeight(0),
    mTileWidth(0), mHeight(0), mWidth(0), mImage(NULL), mPath("")
{
    //Nothing to do here.
}

void ore::Tileset::Clear()
{
    if(mImage)
    {
        mImage->DeleteUser(this);
        mImage = NULL;
    }
    mFirstGid = mLastGid = mTileHeight = mTileWidth = mHeight = mWidth = 0;
    mPath.clear();
    mLocalMgr.Clear();
}

bool ore::Tileset::Load (const std::string& path, ore::uint16 fGid, ore::uint8
                         tileWidth, uint8 tileHeight, ore::ResourceManager *mgr)
{
    //TODO Use the mgr to load/register the image.
    //Getting rid of the old data
    Clear();

    if(path.empty() || !(tileHeight && tileWidth && fGid))
        return true;

    mImage = new Image();
    if(!mImage->GetTexture().loadFromFile(path))
    {
        std::cerr << "Warning: Failed to load image: " << path << "\n";
        delete mImage;
        mImage = NULL;
        return true;
    }

    mImage->AddUser(this);

    //TODO Make it a absolute path before using it as id.
    if(mgr)
        mgr->Register(mImage, path);
    else
        mLocalMgr.Register(mImage);

    //Saving the loaded data into the tileset
    mPath = path;
    mTileHeight = tileHeight;
    mTileWidth = tileWidth;
    mHeight = mImage->GetTexture().getSize().y / tileHeight;
    mWidth = mImage->GetTexture().getSize().x / tileWidth;
    mFirstGid = fGid;
    mLastGid = GetLastGid();
}

sf::Sprite ore::Tileset::GetGidImg(ore::uint16 gid)
{
    if(!mFirstGid || !mWidth || !mImage)
        return sf::Sprite();

    ore::uint16 lid = gid - mFirstGid;
    sf::IntRect crop;
    crop.width = mTileWidth;
    crop.height = mTileHeight;
    crop.left = (lid % mWidth) * mTileWidth;
    crop.top = (lid / mWidth) * mTileHeight;
    return sf::Sprite(mImage->GetTexture(), crop);
}

bool ore::Tileset::Reload()
{
    if(mPath.empty())
        return true;

    sf::Image tmp;
    if(!tmp.loadFromFile(mPath))
        return true;

    mImage->GetTexture().update(tmp);
}
