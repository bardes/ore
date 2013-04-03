#include "Resource.hpp"

ore::Resource::Resource()
{
    mUsers.reserve(32);
}

ore::Resource::~Resource()
{
    //Yay!
}

void ore::Resource::AddUser(Resource *user)
{
    for(int i = 0; i < mUsers.size(); ++i)
        if(mUsers[i] == user)
            return;

    mUsers.push_back(user);
}

bool ore::Resource::DeleteUser(Resource* user)
{
    for(int i = 0; i < mUsers.size(); ++i)
        if(user == mUsers[i])
        {
            mUsers.erase(mUsers.begin() + i);
            return false;
        }
    return true;
}
