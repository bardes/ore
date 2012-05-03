#include "Utils.hpp"

#include <string>
#include <vector>

void ore::base64decode(const std::string &encodedString,
                  std::vector<ore::byte> &decodedData)
{
    //Making sure decodedData is empty before we start...
    decodedData.clear();
    
    //This array tells the integer value of a character based in it's position
    //eg: A=0 B=1 C=3 ....
    char characters[64] = {'A','B','C','D','E','F','G','H','I','J','K','L','M',
                           'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
                           'a','b','c','d','e','f','g','h','i','j','k','l','m',
                           'n','o','p','q','r','s','t','u','v','w','x','y','z',
                           '0','1','2','3','4','5','6','7','8','9','+','/'};
    
    //Is it a valid string
    if(encodedString.length() < 4)
        return;
    
    //Measuring the string (without any paddings)
    unsigned int len = 0;
    while(encodedString[len] != '=' && len < encodedString.length())
        ++len;
    
    //This block gets the 6-bit integer value of the base64 string
    unsigned char values[len];
    unsigned int i; //General iterator
    for (i = 0; i < len; i++)
    {
        unsigned char j = 0;
        for (; encodedString[i] != characters[j]; j++)
        {
            if(j > 63)
                return;
        }
        values[i] = j;
    }
    //Now converting it to 8bit integers (and using a vector to store them)
    for (i = 0; i < len; i += 4)
    {
        if(i + 1 < len)
            decodedData.push_back((values[i] << 2) | (values[i+1] >> 4));
        if(i + 2 < len)
            decodedData.push_back((values[i+1] << 4) | (values[i+2] >>2));
        if(i + 3 < len)
            decodedData.push_back((values[i+2] << 6) | values[i+3]);
    }
}
