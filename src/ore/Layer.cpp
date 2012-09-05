#include "Layer.hpp"

ore::Layer::Layer(ore::uint8 w, ore::uint8 h, const std::string &name) :
mWidth(w), mHeight(h), mName(name)
{
    mGids.reserve(w * h);
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

void ore::Layer::Clear()
{
    mGids.clear();
    mName.clear();
    mWidth = mHeight = 0;
}

