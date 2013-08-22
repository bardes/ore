#include "Object.hpp"

ore::Object::Object() : mLayer(0), mSize(0, 0), mCollisionBox(0, 0, 0, 0),
     mImage(NULL), mAnimLength(0), mAnimSpeedFac(1), mCurrentFrame(0), mSubFrame(0),
     mCrop(0, 0, 0, 0), mOrientation(ore::NORTH)
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
    
    if(p.type == STRING_PROP)
    {
        return *(p.str);
    }
    else if(p.type == NEW_PROP)
    {
        p.type = STRING_PROP;
        p.str = new std::string;
        return *(p.str);
    }
    
    throw ore::PROP_NOT_FOUND;
}

std::vector< ore::uint8 >& ore::Object::GetPropRawData(const std::string& key)
{
    Property &p = GetProp(key);
    
    if(p.type == RAW_DATA_PROP)
    {
        return *(p.raw);
    }
    else if(p.type == NEW_PROP)
    {
        p.type = RAW_DATA_PROP;
        p.raw = new std::vector< ore::uint8 >;
        return *(p.raw);
    }
    
    throw INVALID_PROP_CONVERSION;
}
void ore::Object::ResetAnimation()
{
    if(mSize.x && mImage)
    {
        mAnimLength = mImage->GetTexture().getSize().x / mSize.x;
        mSprite.setTexture(mImage->GetTexture());
    }
    
    mCurrentFrame = mSubFrame = 0;
    mCrop.left = 0;
    mSprite.setTextureRect(mCrop);
}

void ore::Object::SetAnimLength(ore::uint8 l)
{
    if(mImage && mSize.x)
    {
        if(l > mImage->GetTexture().getSize().x * mSize.x)
            mAnimLength = mImage->GetTexture().getSize().x / mSize.x;
        else
            mAnimLength = l;
    }
}

void ore::Object::UpdateAnimation(ore::uint8 frames)
{
    if(!mAnimLength || !mAnimSpeedFac)
        return;
    
    //This is nasty, I know.... But it make sense I swear.
    mSubFrame += frames % mAnimSpeedFac;
    if(mSubFrame >= mAnimSpeedFac)
    {
        mCurrentFrame++;
        mSubFrame %= mAnimSpeedFac;        
    }
    
    mCurrentFrame += frames / mAnimSpeedFac;
    mCurrentFrame %= mAnimLength;
    
    mCrop.left = mCurrentFrame * mSize.x;
    mSprite.setTextureRect(mCrop);
}

bool ore::Object::LoadSprite(const std::string &path)
{
    //Cleaning any old data.
    if(mImage)
        mImage->DeleteUser(this);
    
    //Checking if it isn't already on the resource manager
    Image* loaded;
    loaded = (ore::Image*)mLocalMgr.GetResource(path);
    if(loaded)
    {
        mImage = loaded;
        return true;
    }
    else //If it's not a new one is created
    {
        mImage = new ore::Image;

        if(!mImage->GetTexture().loadFromFile(path))
        {
            delete mImage;
            mImage = NULL;
            return false;
        }
        mLocalMgr.Register(mImage);
    }
    
    //Doing the book keeping...
    mImage->AddUser(this);
    mSprite.setTexture(mImage->GetTexture());
    mSprite.setTextureRect(mCrop);

    if(mSize.x)
        mAnimLength = mImage->GetTexture().getSize().x / mSize.x;
    else
        mAnimLength = 0;
    
    return true;
}

bool ore::Object::LoadSprite(const void* data, size_t size)
{
    //Cleaning any old data...
    if(mImage)
        mImage->DeleteUser(this);
    
    mImage = new ore::Image;
    
    if(!mImage->GetTexture().loadFromMemory(data, size))
    {    
        delete mImage;
        mImage = NULL;
        return false;
    }
    
    mLocalMgr.Register(mImage);
    mImage->AddUser(this);
    mSprite.setTexture(mImage->GetTexture());
    mSprite.setTextureRect(mCrop);

    if(mSize.x)
        mAnimLength = mImage->GetTexture().getSize().x / mSize.x;
    else
        mAnimLength = 0;

    return true;
}
