#include "Layer.hpp"
#include <iostream>

ore::Layer::Layer(ore::uint8 w, ore::uint8 h, ore::uint8 tw, ore::uint8 th, const std::string &name) :
    mWidth(w), mHeight(h), mTileWidth(tw), mTileHeight(th), mName(name)
{
    mGids.reserve(w * h);
}

void ore::Layer::Clear()
{
    mGids.clear();
    mName.clear();
    mCache.clear();
    mWidth = mHeight = 0;
}

sf::Sprite ore::Layer::RenderCache(const std::vector<Tileset*>& tilesets)
{
    mCache.create(mWidth * mTileWidth, mHeight * mTileHeight);
    mCache.clear(sf::Color(0, 0, 0, 0));
    //mCache.setSmooth(false);
    for(int y = 0; y < mHeight; ++y)
    {
        for(int x = 0; x < mWidth; ++x)
        {
            for(int t = 0; t < tilesets.size(); ++t)
            {
                if(tilesets[t]->IsMyGid(GetGid(x, y)))
                {
                    sf::Sprite tile = tilesets[t]->GetGidImg(GetGid(x, y));
                    tile.setPosition((x + 1) * mTileWidth - tilesets[t]->GetTileWidth(),
                                     (y + 1) * mTileHeight - tilesets[t]->GetTileHeight());
                    mCache.draw(tile);
                    break;
                }
            }
        }
    }
    
    mCache.display();
    return sf::Sprite(mCache.getTexture());
}
