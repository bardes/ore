#ifndef BASE64_HPP
#define BASE64_HPP

#include <string>

/**
 * This function takes a encoded string decodes it into a byte array and returns
 * the arry size
 */
int base64decode(const char *encodedString, unsigned char *decodedData,
                 size_t maxDecodedLen);

#endif // BASE64_HPP
