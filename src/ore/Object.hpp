#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "Image.hpp"
#include "Resource.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"

#include <map>
#include <string>
#include <vector>

#include <boost/variant.hpp>

#include <SFML/Graphics.hpp>

namespace ore
{
    class Object : public Resource
    {
    public:
        //TODO Use float precision movement
        Object();
        virtual ~Object();

        virtual ore::RESOURCE_TYPE GetType() const
        {
            return ore::OBJECT_TYPE;
        }

        ore::uint8 GetLayer() const
        {
            return mLayer;
        }
        
//         static ore::Property CreateProp(int n);
//         
//         static ore::Property CreateProp(float n);
//         
//         static ore::Property CreateProp(std::string &str);
//         
//         static ore::Property CreateProp(std::vector<ore::uint8> &raw);
        
        void AddProp(std::string& key, int n)
        {
            mProps[key] = n;
        }
        
        void AddProp(std::string& key, float n)
        {
            mProps[key] = n;
        }
        
        void AddProp(std::string& key, std::string &str)
        {
            mProps[key] = str;
        }
        
        void AddProp(std::string& key, std::vector<ore::uint8> raw)
        {
            mProps[key] = raw;
        }

        std::string& GetPropStr(std::string& key);

        int& GetPropInt(std::string& key);

        float& GetPropFloat(std::string& key);

        std::vector<ore::uint8>& GetPropRawData(std::string& key);

        Property GetProp(std::string& key)
        {
            if(!mProps.count(key))
            {
                Property ret;
                ret.data = NULL;
                ret.type = UNKNOWN_PROP;
                return ret;
            }

            return mProps[key];
        }
        
        bool DeleteProp(std::string &key);

        void SetLayer(ore::uint8 l)
        {
            mLayer = l;
        }

        void SetPos(sf::Vector2i p)
        {
            //mPos = p;
            mSprite.setPosition(p.x, p.y);
        }

        void SetPos(ore::int32 x, ore::int32 y)
        {
            mSprite.setPosition(x, y);
        }

        void SetWidth(ore::uint32 w)
        {
            mSize.x = mCrop.width = w;
        }

        void SetHeight(ore::uint32 h)
        {
            mSize.y = mCrop.height = h;
        }

        void SetSize(ore::uint32 w, ore::uint32 h)
        {
            mSize.x = mCrop.width = w;
            mSize.y = mCrop.height = h;
        }

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

        ore::Property Prop(const std::string &key)
        {
            return mProps[key];
        }

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

//       private:

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
         * @brief Rady to draw sprite of the object image, also stores the
         * object's position
         */
        sf::Sprite mSprite;

        /**
         * @brief Used to crop the sprite image;
         */
        sf::IntRect mCrop;

        /**
         * @brief Direction of the object.
         */
        ore::ORIENTATION mOrientation;

        /**
         * @brief Properties of this object.
         */
        std::map<const std::string, boost::variant<int, float, std::string,
                                             std::vector<ore::uint8> > > mProps;

        /**
         * @brief Loacal resource manager.
         */
        ResourceManager mLocalMgr;
    };
}

#endif // OBJECT_HPP
