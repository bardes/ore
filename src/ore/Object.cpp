#include "Object.hpp"

ore::Object::Object() : mLayer(0), mSize(0, 0), mCollisionBox(0, 0, 0, 0),
    mOrigin(0, 0), mCrop(0, 0, 0, 0), mOrientation(ore::NORTH)
{
}

ore::Object::~Object()
{
    while(mProps.begin() != mProps.end())
    {
//         switch(mProps.begin()->second.type)
//         { 
//             case ore::STRING_PROP: delete mProps.begin()->second.data.str; break;    
//             case ore::RAW_DATA_PROP: delete mProps.begin()->second.data.raw; break;
//             default: break;
//         }
        mProps.erase(mProps.begin());
    }
}

ore::Property ore::Object::CreateProp(std::string &str)
{
    Property p;
    p.type = STRING_PROP;
    p.data = str;
    return p;
}

ore::Property ore::Object::CreateProp(float n)
{
    Property p;
    p.type = FLOAT_PROP;
    p.data = n;
    return p;
}

ore::Property ore::Object::CreateProp(int n)
{
    Property p;
    p.type = INT_PROP;
    p.data = n;
    return p;
}

ore::Property ore::Object::CreateProp(std::vector<ore::uint8> &raw)
{
    Property p;
    p.type = RAW_DATA_PROP;
    p.data = std::vector<ore::uint8>(raw);
    return p;
}

bool ore::Object::DeleteProp(std::string &key)
{
    std::map<std::string, ore::Property>::iterator i;
    if((i = mProps.find(key)) == mProps.end())
        return false;
    
//     switch(i->second.type)
//     {
//         case ore::INT_PROP: delete (int*)i->second.data; break;    
//         case ore::FLOAT_PROP: delete (float*)i->second.data; break;    
//         case ore::STRING_PROP: delete (std::string*)i->second.data; break;    
//         case ore::RAW_DATA_PROP: delete (std::vector<ore::uint8>*)i->second.data; break;
//         default: break;
//     }
    
    mProps.erase(i);
    return true;
}

void ore::Object::RestartAnimation()
{
    mCrop.left = 0;
    mSprite.setTextureRect(mCrop);
    mSprite.setTexture(mImage->GetTexture());
}

void ore::Object::UpdateAnimation(ore::uint8 frames)
{
    mCrop.left += frames * mSize.x;
    if(mCrop.left >= mImage->GetTexture().getSize().x)
        mCrop.left = 0;
    mSprite.setTextureRect(mCrop);
}

std::string* ore::Object::GetPropStr(std::string& key)
{
    if(!mProps.count(key))
        return NULL;

    if(mProps[key].type != STRING_PROP)
        return NULL;

    return (std::string*)(mProps[key].data);
}

int* ore::Object::GetPropInt(std::string& key)
{
    if(!mProps.count(key))
        return NULL;

    if(mProps[key].type != INT_PROP)
        return NULL;

    return (int*)(mProps[key].data);
}

float* ore::Object::GetPropFloat(std::string& key)
{
    if(!mProps.count(key))
        return NULL;

    if(mProps[key].type != FLOAT_PROP)
        return NULL;

    return (float*)(mProps[key].data);
}

std::vector<ore::uint8>* ore::Object::GetPropRawData(std::string& key)
{
    if(!mProps.count(key))
        return NULL;

    if(mProps[key].type != RAW_DATA_PROP)
        return NULL;

    return (std::vector<ore::uint8>*)(mProps[key].data);
}
