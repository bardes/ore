#include "Utils.hpp"

namespace ore
{
    /**
    * This function takes an encoded string and decodes it into an ore::uint8
    * vector.\n
    * \a decodedData will be an empty vector in case of failure.
    */
    void DecodeBase64(const std::string &encodedString,
                    std::vector<ore::uint8> &decodedData);
}