#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include "Utils.hpp"

#include <vector>

namespace ore
{
    class Resource
    {
    public:
        virtual ~Resource();
        /**
         * @brief When implemented should return the type of the resource.
         */
        virtual RESOURCE_TYPE GetType() const = 0;

        /**
         * @brief Adds a new user.
         * @param user Address of the new user.
         */
        void AddUser(Resource* user);

        /**
         * @brief Removes the specified user from the list.
         * @param user Address of the user to be deleted.
         * @return True in case of failure.
         */
        bool DeleteUser(Resource* user);

        /**
         * @brief Gets the address of each user of this resource.
         * @return This returns a reference to a std::vector with the addresses.
         */
        const std::vector<const Resource*>& GetUsers()const
        {
            return mUsers;
        }

    protected:
        Resource();

    private:

        /**
         * @brief Users of this resource.
         */
        std::vector<const Resource*> mUsers;
    };
}
#endif // RESOURCE_HPP
