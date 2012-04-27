#include "base64.hpp"

#include <vector>

int base64decode(const char *encodedString, unsigned char *decodedData,
                 size_t maxDecodedLen)
{
    //this array teels the integer value of a character based in it's position
    //eg: A=0 B=1 C=3 ....
    char characters[64] = {'A','B','C','D','E','F','G','H','I','J','K','L','M',
                           'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
                           'a','b','c','d','e','f','g','h','i','j','k','l','m',
                           'n','o','p','q','r','s','t','u','v','w','x','y','z',
                           '0','1','2','3','4','5','6','7','8','9','+','/'};
    std::vector<char> decoded;
    
    //measuring the string
    int len = 0;
    while(encodedString[len] != '\0' && encodedString[len] != '=')
        ++len;
    
    if(len < 2)
            return 0;
    
    //this block gets the 6-bit integer value of the base64 string
    unsigned char values[len];
    for (unsigned int i = 0; i < len; i++)
    {
        unsigned char j = 0;
        for (; encodedString[i] != characters[j]; j++)
        {
            if(j > 63)
                return 0;
        }
        values[i] = j;
    }
    
    //now converting it to 8bit integers (and using a vector to store them)
    for (unsigned int i = 0; i < len; i += 4)
    {
        if(i + 1 < len)
            decoded.push_back(((values[i] << 6) | values[i + 1]) >> 4);
        if(i + 2 < len)
            decoded.push_back(((values[i + 1] % 16) << 4) | values[i + 2] >> 2);
        if(i + 3 < len)
            decoded.push_back(((values[i + 2] % 4) << 6) | values[i + 3]);
    }

    //Will it fit?
    if(decoded.size() > maxDecodedLen)
        return 0;

    //Copying the values
    for (int i = 0; i < decoded.size(); ++i)
        decodedData[i] = decoded[i];

    return decoded.size();
}
