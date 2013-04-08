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

//TODO change all functions that return false on success to true;
namespace ore
{
    class Object : public Resource
    {
    public:
        Object();
        virtual ~Object();

        virtual ResourceType GetType() const
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
        void SetWidth(float w)
        {
            mSize.x = mCrop.width = w;
        }

        /**
         * @brief Sets the height of the object.
         * 
         * Note: this is only used for drawing the object. Collision detection is
         * performed using a collision box.
         */
        void SetHeight(float h)
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
        void SetSize(float w, float h)
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
        void SetSize(sf::Vector2f size)
        {
            mSize = size;
            mCrop.width = size.x;
            mCrop.height = size.y;
        }

        /**
         * @brief Gets the width of the object.
         */
        float GetWidth() const
        {
            return mSize.x;
        }

        /**
         * @brief Gets the height of the object.
         */
        float GetHeight() const
        {
            return mSize.y;
        }

        /**
         * @brief Gets the size of the object.
         */
        sf::Vector2f GetSize() const
        {
            return mSize;
        }
        
        /**
         * @brief Moves the object.
         * @param delta How much to move the object.
         */
        void Move(const sf::Vector2f& delta)
        {
            mSprite.move(delta);
        }

        /**
         * @brief Moves the object.
         * @param x Amount of movement on the x axis.
         * @param y Amount of movement on the y axis.
         */
        void Move(float x, float y)
        {
            mSprite.move(x, y);
        }

        /**
         * @brief Sets which direction the object is facing.
         * 
         * Note that combinations of directions are acceptable:
         * \code
         * obj.SetOrientation(ore::NORTH | ore::EAST) // --> Facing Northeast
         * \endcode
         */
        void SetOrientation(ore::uint8 o)
        {
            mOrientation = o;
        }

        /**
         * @brief Gets the orientation value of the object.
         * 
         * Remember that ored values are possible, so be prepared to check for
         * combinations of orientations.
         */
        ore::uint8 GetOrientation() const
        {
            return mOrientation;
        }

        /**
         * @brief Sets up the collision box for this object.
         * 
         * Coordinates are relative to the object and measured in pixels.
         */
        void SetCollisionBox(const sf::FloatRect& box)
        {
            mCollisionBox = box;
        }

        /**
         * @brief Gets a copy of the collision box.\n
         * Don't forget that the position returned is relative to the object!
         */
        sf::FloatRect GetCollisionBox() const
        {
            return mCollisionBox;
        }

        /**
         * @brief Sets the origin point of the object.
         * 
         * This is the point used to calculate distances, angles, rotations etc...
         * \n Don't forget that the coordinates are relative to the object.
         */
        void SetOrigin(float x, float y)
        {
            mSprite.setOrigin(x, y);
        }

        /**
         * @brief Gets the origin point of the object. (relative to the object)
         * 
         * This is the point used to calculate distances, angles, rotations etc...
         * \n Don't forget that the coordinates are relative to the object.
         */
        sf::Vector2f GetOrigin() const
        {
            return mSprite.getOrigin();
        }
        
        //TODO Implement the function and put a link to the specification  in the comment below.
        /**
         * @brief Loads an object from a ".lua" file.
         * 
         * The file should contain the necessary properties and functions to
         * create an instance the object and follow this specifications.
         * @param file Path to the .lua file.
         * @param mgr This may be used to specify a different resource manager to
         * take care of the contents created during the loading process.
         */
        bool Load(const std::string& file, ResourceManager* mgr = NULL);

        /**
         * @brief Sets the animation type.
         * 
         * An object may have several different animation types. Each one being
         * one row on the sprite image. You can change between them by using this
         * function.
         */
        void SetAnimationType(ore::uint8 animType)
        {
            mCrop.top = animType * mSize.y;
        }

        /**
         * @brief Resets the current animation to the first frame.
         * 
         * This does _NOT_ change the animation type.
         */
        void ResetAnimation();

        /**
         * @brief Updates the animation of this object.
         * 
         * You probably won't need to call this with any argument, unless you
         * have frame dropping.\n
         * Note that if the sprite of the object is not set up properly this won't
         * work.
         * @param frames Number of frames you want to advance.
         */
        void UpdateAnimation(ore::uint8 frames = 1);
        
        /**
         * @brief Sets how many times slower the animation will run.
         * 
         * Setting this to 0 will result in a non animated object.
         */
        void SetAnimSpeedFac(ore::uint8 s)
        {
            mAnimSpeedFac = s;
        }
        
        /**
         * @brief Sets the lenght of the animation of this object.
         * 
         * This is the number of actual frames, __without__ accounting for sub frames...\n
         * __ATENTION__: The size of the object must be set before this function
         * call, otherwise nothing will happen.
         */
        void SetAnimLength(ore::uint8 l);

        /**
         * @brief Gets a "ready to draw" sprite image of the object.
         * 
         * For this to be really up to date UpdateAnimation()
         * Note that the sprite is already positioned correctly so there is no
         * need to move it before drawing.
         */
        sf::Sprite Sprite()
        {
            return mSprite;
        }
        
        /**
         * @brief Loads the sprite image from an image file.
         * 
         * This function will attempt to set the animation length based on the
         * object size and the image size. If mSize is not set mAnimLength will
         * become 0.\n
         * Note that if you want to do a reload you should use the Reload() function
         * since this won't actually reload the file if it's found in the resMgr.
         */
        bool LoadSprite(const std::string& path);
        
        /**
         * @brief Loads a sprite image directly from memory.
         * 
         * This function will attempt to set the animation length based on the
         * object size and the image size. If mSize is not set mAnimLength will
         * become 0.
         * @param data This should be an array of pixels using the RGBA format.
         * @param size Size of the array. (in bytes)
         */
        bool LoadSprite(const void* data, size_t size);
        
      private:

        /**
         * @brief Layer at which this object should be drawn.
         */
        ore::uint8 mLayer;

        /**
         * @brief Size of the object. (in pixels)
         */
        sf::Vector2f mSize;

        /**
         * @brief Area considered solid during collisions
         */
        sf::FloatRect mCollisionBox;

        /**
         * @brief Sprite image of the object.
         */
        ore::Image *mImage;
        
        /**
         * @brief Length of the animation. (in frames)
         */
        ore::uint8 mAnimLength;
        
        /**
         * Animation speed will be divided by this factor.
         */
        ore::uint8 mAnimSpeedFac;
        
        /**
         * @brief Actual frame of the animation.
         * 
         * This tells in which animation frame this object is.
         */
        ore::uint8 mCurrentFrame;
        
        /**
         * @brief This is used to count frames that don't update the animation.
         * 
         * Animation is only updated when the mSubFrame is equals to mAnimSpeedFac
         * this makes sure that only one every x frames result on an update. (x 
         * being defined by mAnimSpeedFac)
         */
        ore::uint8 mSubFrame;

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
        ore::uint8 mOrientation;

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
