#include "Layer.hpp"

ore::Layer::Layer(ore::uint8 width, ore::uint8 height)
{
    mGids.reserve(width*height);
}

ore::uint16& ore::Layer::operator[](ore::uint16 index)
{
    if(index >= mWidth * mHeight)
        throw ore::INDEX_OUT_OF_RANGE;
    return mGids[index];
}

const ore::uint16& ore::Layer::operator[](ore::uint16 index) const
{
    if(index >= mWidth * mHeight)
        throw ore::INDEX_OUT_OF_RANGE;
    return mGids[index];
}


