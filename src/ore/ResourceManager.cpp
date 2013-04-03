#include "ResourceManager.hpp"
#include "Resource.hpp"

ore::ResourceManager::ResourceManager()
{
    //Avoid reallocations
    mAnonymousResources.reserve(64);
}

ore::ResourceManager::~ResourceManager()
{
    for(int i = 0; i < mAnonymousResources.size(); ++i)
        delete mAnonymousResources[i];

    std::map<const std::string, Resource*>::const_iterator itr;
    for(itr = mIdResources.begin(); itr != mIdResources.end(); ++itr)
        delete itr->second;
}

void ore::ResourceManager::Register(Resource *res, const std::string &id)
{
    if(id.empty())
    {
        //Preventing duplicates
        for(int i = 0; i < mAnonymousResources.size(); ++i)
            if(mAnonymousResources[i] == res)
                return;

        mAnonymousResources.push_back(res);
    }
    else
    {
        //Preventing duplicates
        if(mIdResources.find(id) != mIdResources.end())
            return;

        mIdResources[id] = res;
    }
}

bool ore::ResourceManager::Unregister(Resource *res)
{
    for(int i = 0; i < mAnonymousResources.size(); ++i)
        if(mAnonymousResources[i] == res)
        {
            mAnonymousResources.erase(mAnonymousResources.begin() + i);
            return false;
        }
    return true;
}

bool ore::ResourceManager::Unregister(const std::string& id)
{
    std::map<const std::string, Resource*>::iterator i;
    if((i = mIdResources.find(id)) == mIdResources.end())
        return true;

    mIdResources.erase(i);
    return false;
}

bool ore::ResourceManager::Delete(const Resource *res, bool force)
{
    //Don't delete unless it really needs to
    if(!force && res->GetUsers().size() > 0)
        return true;

    //Finding it
    for(int i = 0; i < mAnonymousResources.size(); ++i)
        if(mAnonymousResources[i] == res)
        {
            delete mAnonymousResources[i];
            mAnonymousResources.erase(mAnonymousResources.begin() + i);
            return false;
        }
    return true;
}

bool ore::ResourceManager::Delete(const std::string& id, bool force)
{
    //Finding it
    std::map<const std::string, Resource*>::iterator i;
    if((i = mIdResources.find(id)) == mIdResources.end())
        return true;

    //Don't delete unless it really needs to
    if(!force && i->second->GetUsers().size() > 0)
        return true;

    delete i->second; //Hasta la vista baby!
    mIdResources.erase(i);

    return false;
}

void ore::ResourceManager::Clear()
{
    for(int i = 0; i < mAnonymousResources.size(); ++i)
        delete mAnonymousResources[i];

    std::map<const std::string, Resource*>::const_iterator i;
    for(i = mIdResources.begin(); i != mIdResources.end(); ++i)
        delete i->second;

    mAnonymousResources.clear();
    mIdResources.clear();
}

const ore::Resource* ore::ResourceManager::GetResource(const std::string &id) const
{
    std::map<const std::string, Resource*>::const_iterator i;
    if((i = mIdResources.find(id))  == mIdResources.end())
        return NULL;
    return i->second;
}

ore::Resource* ore::ResourceManager::GetResource(const std::string &id)
{
    std::map<const std::string, Resource*>::const_iterator i;
    if((i = mIdResources.find(id)) == mIdResources.end())
        return NULL;
    return i->second;
}
