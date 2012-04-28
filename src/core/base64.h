#ifndef BASE64_H
#define BASE64_H

/**
 * This function takes an encoded string and decodes it into a byte array.
 * Return value: Size of the decoded array.
 **/
int base64decode(const char *encodedString, unsigned char *decodedData,
                 size_t maxDecodedLen);

#endif // BASE64_H
