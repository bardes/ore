#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include "Utils.hpp"

#include <map>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

namespace ore
{
    class Resource;

    class ResourceManager
    {
    public:
        ResourceManager();

        ~ResourceManager();
        
        /**
         * @brief Registers a new resource to this manager.
         * The usr option is a convenience to join the AddUser() and Register()
         * functions, since usually they are used together.
         * @param res Resource to be monitored by this manager.
         * @param usr The user of this resource. If set to NULL wil be ignored.
         */
        void Register(Resource *res, const std::string& id = "");

        /**
         * @brief Unregisters a resource without deleting it.
         * This function simply makes the the manager forget about this
         * resource. It doesn't erase anything.
         * @param res Resource to be unregistered.
         * @return True in case of failure.
         */
        bool Unregister(Resource *res);

        /**
         * @brief Unregisters a resource _without_ deleting it.
         * This function simply makes the the manager forget about this
         * resource. It doesn't erase anything.
         * @param id Id of the resource to be unregistered.
         * @return True in case of failure.
         */
        bool Unregister(const std::string& id);

        /**
         * @brief Deletes the resource from the manager and erases it's content.
         * This function attempts to delete a resource but by default will stop if
         * there are still others using it. The force flag may be used to overide
         * this behavior.
         * @param res Resource to be deleted.
         * @param force If set to true will force to delete even if there are other
         * users registered to this resource.
         * @return Returns true if the resource could _NOT_ be erased or found.
         */
        bool Delete(const Resource *res, bool force = false);

        /**
         * @brief Deletes the resource from the manager and erases it's content.
         * This function attempts to delete a resource but by default will stop if
         * there are still others using it. The force flag may be used to overide
         * this behavior.
         * @param id Id of the resource to be deleted.
         * @param force If set to true will force to delete even if there are other
         * users registered to this resource.
         * @return Returns true if the resource could _NOT_ be erased or found.
         */
        bool Delete(const std::string& id, bool force = false);

        /**
         * @brief Deletes all unused resources.
         */
        //TODO void Clean();

        /**
         * @brief Forces all data of this manager to be deleted.
         */
        void Clear();

        /**
         * @brief Gets the number of resources of this manager.
         */
        ore::uint32 GetTotResourcesNbr() const
        {
            return mAnonymousResources.size() + mIdResources.size();
        }

        /**
         * @brief Get a resource based on it's id.
         * @param id Id of the resource.
         * @return The address of the resource or NULL if not found.
         */
        const Resource* GetResource(const std::string &id) const;
        Resource* GetResource(const std::string &id);

    private:

        std::vector<Resource*> mAnonymousResources;

        std::map<const std::string, Resource*> mIdResources;
    };
}
#endif // RESOURCEMANAGER_HPP
