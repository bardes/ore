#include "Object.hpp"

ore::Object::Object() : mLayer(0), mSize(0, 0), mCollisionBox(0, 0, 0, 0),
    mOrigin(0, 0), mCrop(0, 0, 0, 0), mOrientation(ore::NORTH)
{
}

ore::Object::~Object()
{
    while(mProps.begin() != mProps.end())
    {
        switch(mProps.begin()->second.type)
        { 
            case ore::STRING_PROP: delete mProps.begin()->second.str; break;    
            case ore::RAW_DATA_PROP: delete mProps.begin()->second.raw; break;
            default: break;
        }
        mProps.erase(mProps.begin());
    }
}


void ore::Object::SetProp(const std::string& key, int n)
{
    //Deleting any possible old data...
    DeleteProp(key);
    
    Property p;
    p.type = INT_PROP;
    p.i = n;
    mProps[key] = p;
}

void ore::Object::SetProp(const std::string& key, float n)
{
    //Deleting any possible old data...
    DeleteProp(key);
    
    Property p;
    p.type = FLOAT_PROP;
    p.f = n;
    mProps[key] = p;
}

void ore::Object::SetProp(const std::string& key, const std::string& str)
{
    //Deleting any possible old data...
    DeleteProp(key);
    
    Property p;
    p.type = STRING_PROP;
    p.str = new std::string(str);
    mProps[key] = p;
}

void ore::Object::SetProp(const std::string& key, const std::vector< ore::uint8 >& raw)
{
    //Deleting any possible old data...
    DeleteProp(key);
    
    Property p;
    p.type = RAW_DATA_PROP;
    p.raw = new std::vector<ore::uint8>(raw);
    mProps[key] = p;
}

void ore::Object::DeleteProp(const std::string& key)
{
    std::map< const std::string, Property >::iterator i;
    if((i = mProps.find(key)) == mProps.end())
        return;

    //Cleaning up any memory being used by the prop...
    switch(i->second.type)
    {
        case STRING_PROP: delete i->second.str; break;
        case RAW_DATA_PROP: delete i->second.raw; break;
        case UNKNOWN_PROP: break; //TODO print a warning.
        default: break;
    }
    
    mProps.erase(i); //Bye bye :)
}

ore::Property& ore::Object::GetProp(const std::string& key)
{
    std::map< std::string, Property >::iterator i;
    
    if((i = mProps.find(key)) == mProps.end())
    {
        mProps[key] = Property();
        return mProps[key];
    }
    
    return i->second;
}

int& ore::Object::GetPropInt(const std::string& key)
{
    Property &p = GetProp(key);
    if(p.type == INT_PROP)
    {
        return p.i;
    }
    else if(p.type == NEW_PROP)
    {
        p.type = INT_PROP;
        return p.i;
    }
    
    throw INVALID_PROP_CONVERSION;
}

float& ore::Object::GetPropFloat(const std::string& key)
{
    Property &p = GetProp(key);
    
    if(p.type == FLOAT_PROP)
    {
        return p.f;
    }
    else if(p.type == NEW_PROP)
    {
        p.type = FLOAT_PROP;
        return p.f;
    }
    
    throw INVALID_PROP_CONVERSION;
}

std::string& ore::Object::GetPropStr(const std::string& key)
{
    Property &p = GetProp(key);
    if(p.type != STRING_PROP)
        throw ore::PROP_NOT_FOUND;
    
    return *(p.str);
}

std::vector< ore::uint8 >& ore::Object::GetPropRawData(const std::string& key)
{
    Property &p = GetProp(key);
    if(p.type != RAW_DATA_PROP)
        throw INVALID_PROP_CONVERSION;
    
    return *(p.raw);
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

bool ore::Object::LoadSprite(const std::string &path)
{
    if(mImage)
        mImage->DeleteUser(this);
    
    Image* loaded;
    loaded = (ore::Image*)mLocalMgr.GetResource(path);
    
    if(loaded)
    {
        mImage = loaded;
        mImage->AddUser(this);
        return true;
    }
    
    mImage = new ore::Image;
    
    if(!mImage->GetTexture().loadFromFile(path))
    {
        delete mImage;
        mImage = NULL;
        return false;
    }
    
    mLocalMgr.Register(mImage);
    mImage->AddUser(this);
    return true;
}
