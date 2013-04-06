#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "Image.hpp"
#include "Resource.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"

#include <map>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

//TODO change all functions thar return false on success to true;
namespace ore
{
    class Object : public Resource
    {
    public:
        //TODO Use float precision movement
        Object();
        virtual ~Object();

        virtual RESOURCE_TYPE GetType() const
        {
            return OBJECT_TYPE;
        }

        ore::uint8 GetLayer() const
        {
            return mLayer;
        }
        
        /**
         * @brief Checks if an object has the property specified by the given \a key.
         * 
         * Note that this function doesn't perform any aditional checking, so 
         * even "UNKNOWN_TYPE" and uninitialized properties return true.
         * @param key Key to be checked.
         * @return true if the key is found.
         */
        bool HasProp(const std::string &key)
        {
            return mProps.count(key);
        }
        
        /**
         * @brief Sets up an integer property.
         * 
         * This function changes the value of an exiting property or creates one
         * if none is found. Note that this functions also changes the property
         * type, so it's ok to set different types to the same prop.
         * @param key String that identifies the property. (A.K.A. key)
         * @param n Value of the property.
         */
        void SetProp(const std::string& key, int n);
        
        /**
         * @brief Sets up a float property.
         * 
         * This function changes the value of an exiting property or creates one
         * if none is found.
         * __ATENTION__: If you want a float prop don't forget to append the "f"
         * suffix to the number. Otherwise it may be ambiguous to the compiler.
         * @param key String that identifies the property. (A.K.A. key)
         * @param n Value of the property.
         */
        void SetProp(const std::string& key, float n);
        
        /**
         * @brief Sets up a string property.
         * 
         * This function changes the value of an exiting property or creates one
         * if none is found. Note that this functions also changes the property
         * type, so it's ok to set different types to the same prop.
         * @param key String that identifies the property. (A.K.A. key)
         * @param str Value of the property.
         */
        void SetProp(const std::string& key, const std::string& str);
        
        /**
         * @brief Sets up a raw property.
         * 
         * This function changes the value of an exiting property or creates one
         * if none is found. Note that this functions also changes the property
         * type, so it's ok to set different types to the same prop.
         * Side note: A raw property is just a vector of ore::uint8 (bytes basicaly).
         * @param key String that identifies the property. (A.K.A. key)
         * @param raw Value of the property.
         */
        void SetProp(const std::string& key, const std::vector<ore::uint8>& raw);
        
        /**
         * @brief Gets a reference to the prop defined by \a key.
         * 
         * If there's no property defined by \a key, one is created with type
         * \a ore::NEW_PROP and returned.\n
         * _CAUTION_: There is no type checking when accessing the property data
         * through this function.
         * @param key Key of the desired value.
         */
        Property& GetProp(const std::string& key);

        /**
         * @brief Gets a reference to an integer prop.
         * 
         * If \a key doesn't exist already a new prop is created.\n
         * Note tha this function DOES perform a type check using the value of
         * "prop.type", and thus may trhow an exeption if it's not an integer.
         * @param key Key of the desired value.
         */
        int& GetPropInt(const std::string& key);

        /**
         * @brief Gets a reference to a float prop.
         * 
         * If \a key doesn't exist already a new prop is created.\n
         * Note tha this function DOES perform a type check using the value of
         * "prop.type", and thus may trhow an exeption if it's not a float.
         * @param key Key of the desired value.
         */
        float& GetPropFloat(const std::string& key);

        /**
         * @brief Gets a reference to a string prop.
         * 
         * If \a key doesn't exist already a new prop is created.\n
         * Note tha this function DOES perform a type check using the value of
         * "prop.type", and thus may trhow an exeption if it's not a string.
         * @param key Key of the desired value.
         */
        std::string& GetPropStr(const std::string& key);

        /**
         * @brief Gets a reference to a "raw data" prop.
         * 
         * If \a key doesn't exist already a new prop is created.\n
         * Note tha this function DOES perform a type check using the value of
         * "prop.type", and thus may trhow an exeption if it's not "raw data".
         * @param key Key of the desired value.
         */
        std::vector< ore::uint8 >& GetPropRawData(const std::string& key);

        /**
         * @brief Deletes the given prop and erases any content allocated by it.
         * 
         * If the prop doesn't exist nothing is done.
         * @param key Key of the desired value.
         */
        void DeleteProp(const std::string& key);

        /**
         * @brief Sets the object't current layer.
         */
        void SetLayer(ore::uint8 l)
        {
            mLayer = l;
        }

        /**
         * @brief Sets the current position of the object.
         */
        void SetPos(sf::Vector2i p)
        {
            mSprite.setPosition(p.x, p.y);
        }

        /**
         * @brief Sets the current position of the object.
         */
        void SetPos(ore::int32 x, ore::int32 y)
        {
            mSprite.setPosition(x, y);
        }

        /**
         * @brief Sets the width of the object.
         * 
         * Note: this is only used for drawing the object. Collision detection is
         * performed using a collision box.
         */
        void SetWidth(ore::uint32 w)
        {
            mSize.x = mCrop.width = w;
        }

        /**
         * @brief Sets the height of the object.
         * 
         * Note: this is only used for drawing the object. Collision detection is
         * performed using a collision box.
         */
        void SetHeight(ore::uint32 h)
        {
            mSize.y = mCrop.height = h;
        }

        /**
         * @brief Sets the size of the object. (same as obj.SetWidth(w) and 
         * obj.SetHeight(h))
         * 
         * Note: this is only used for drawing the object. Collision detection is
         * performed using a collision box.
         */
        void SetSize(ore::uint32 w, ore::uint32 h)
        {
            mSize.x = mCrop.width = w;
            mSize.y = mCrop.height = h;
        }

        /**
         * @brief Sets the size of the object. (same as obj.SetWidth(w) and 
         * obj.SetHeight(h))
         * 
         * Note: this is only used for drawing the object. Collision detection is
         * performed using a collision box.
         */
        void SetSize(sf::Vector2i size)
        {
            mSize = size;
            mCrop.width = size.x;
            mCrop.height = size.y;
        }

        ore::uint32 GetWidth() const
        {
            return mSize.x;
        }

        ore::uint32 GetHeight() const
        {
            return mSize.y;
        }

        sf::Vector2i GetSize() const
        {
            return mSize;
        }

        void Move(sf::Vector2i delta)
        {
            mSprite.move(delta.x, delta.y);
        }

        void Move(float x, float y)
        {
            mSprite.move(x, y);
        }

        void SetOrientation(ore::ORIENTATION o)
        {
            mOrientation = o;
        }

        ore::ORIENTATION GetOrientation() const
        {
            return mOrientation;
        }

        void SetCollisionBox(const sf::IntRect& box)
        {
            mCollisionBox = box;
        }

        sf::IntRect GetCollisionBox() const
        {
            return mCollisionBox;
        }

        void SetOrigin(ore::uint32 x, ore::uint32 y)
        {
            mOrigin.x = x;
            mOrigin.y = y;
        }

        sf::Vector2i GetOrigin() const
        {
            return mOrigin;
        }
        
        //TODO Implement the function and put a link to the specification  in the comment below.
        /**
         * @brief Loads an object from a ".lua" file.
         * 
         * The file should contain the necessary properties and functions to
         * create an instance the object, following this especifications.
         * @param file Path to the .lua file.
         * @param mgr This may be used to specify a different resource manager to
         * take care of the contents created during the loading process.
         */
        bool Load(const std::string& file, ResourceManager* mgr = NULL);

        void SetAnimationType(ore::uint8 animType)
        {
            mCrop.top = animType * mSize.y;
        }

        void RestartAnimation();

        void UpdateAnimation(ore::uint8 frames = 1);

        sf::Sprite Draw()
        {
            return mSprite;
        }
        
        /**
         * @brief Loads the sprite image from an image file.
         * 
         * Note that if you want to do a reload you should use the Reload() function.
         */
        bool LoadSprite(const std::string& path);
        
        bool LoadSprite(const void* data, size_t size)
        {
            return mImage->GetTexture().loadFromMemory(data, size);
        }
      private:

        /**
         * @brief Layer at which this object should be drawn.
         */
        ore::uint8 mLayer;

        /**
         * @brief Size of the object. (in pixels)
         */
        sf::Vector2i mSize;

        /**
         * @brief Area considered solid during collisions
         */
        sf::IntRect mCollisionBox;

        /**
         * @brief Origin point used to measure distances.
         */
        sf::Vector2i mOrigin;

        /**
         * @brief Sprite image of the object.
         */
        ore::Image *mImage;

        /**
         * @brief A "ready to draw" sprite of the object image, also stores the
         * object's position
         */
        sf::Sprite mSprite;

        /**
         * @brief Used to crop the sprite image, and animate.
         */
        sf::IntRect mCrop;

        /**
         * @brief Direction of the object.
         */
        ore::ORIENTATION mOrientation;

        /**
         * @brief Properties of this object.
         */
        std::map< const std::string, Property > mProps;

        /**
         * @brief Loacal resource manager.
         */
        ResourceManager mLocalMgr;
    };
}

#endif // OBJECT_HPP
