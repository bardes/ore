#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "Resource.hpp"
#include "Utils.hpp"

#include "SFML/Graphics.hpp"

#include <string>

namespace ore
{
    class Image : public Resource
    {
    public:
        Image();
        virtual ~Image();

        virtual ResourceType GetType() const
        {
            return IMAGE_TYPE;
        }

        sf::Texture& GetTexture()
        {
            return mTexture;
        }

        const sf::Texture& GetTexture() const
        {
            return mTexture;
        }

    private:

        sf::Texture mTexture;
    };
}
#endif // IMAGE_HPP
