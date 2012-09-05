#include "Utils.hpp"

namespace ore
{
    /**
    * This function takes an encoded string and decodes it into an ore::uint8
    * vector. decodedData will be an empty vector on failure.
    **/
    void base64decode(const std::string &encodedString,
                    std::vector<ore::uint8> &decodedData);
}