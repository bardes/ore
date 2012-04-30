#ifndef BASE64_HPP
#define BASE64_HPP

#include <string>
#include <vector>

typedef unsigned char Byte;

/**
 * This function takes an encoded string and decodes it into a unsigned char
 * vector. decodedData will be an empty vector on failure.
 **/
void base64decode(const std::string& encodedString,
                  std::vector<Byte> &decodedData);

#endif // BASE64_HPP
