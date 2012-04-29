int base64decode(const char *encodedString, unsigned char *decodedData,
                 unsigned int maxDecodedLen)
{
    //this array teels the integer value of a character based in it's position
    //eg: A=0 B=1 C=3 ....
    char characters[64] = {'A','B','C','D','E','F','G','H','I','J','K','L','M',
                           'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
                           'a','b','c','d','e','f','g','h','i','j','k','l','m',
                           'n','o','p','q','r','s','t','u','v','w','x','y','z',
                           '0','1','2','3','4','5','6','7','8','9','+','/'};
    
    //Measuring the string (Does NOT include the paddings)
    int len = 0;
    while(encodedString[len] != '\0' && encodedString[len] != '=')
        ++len;
    
    //Is it valid
    if(len < 2)
            return 0;
    
    //This block gets the 6-bit integer value of the base64 string
    unsigned char values[len];
    unsigned int i; //General iterator
    for (i = 0; i < len; i++)
    {
        unsigned char j = 0;
        for (; encodedString[i] != characters[j]; j++)
        {
            if(j > 63)
                return 0;
        }
        values[i] = j;
    }
    
    //Calculating the decoded lenght
    int decodedLen;
    switch(len % 4)
    {
        case 0: decodedLen = (len / 4) * 3; break; //No padding
        case 2: decodedLen = ((len / 4) * 3) + 1; break; //Two paddings (==)
        case 3: decodedLen = ((len / 4) * 3) + 2; break; //One padding  (=)
        default: return 0; break; //Should not happen in normal conditions
    }

    //Will it fit?
    if(decodedLen > maxDecodedLen)
        return 0;
    
    //Now converting it to 8bit integers (and using a vector to store them)
    for (i = 0; i < len; i += 4)
    {
        if(i + 1 < len)
            decodedData[(i/4) * 3] = (values[i] << 2) | (values[i+1] >> 4);
        if(i + 2 < len)
            decodedData[((i / 4) * 3)+1] = (values[i+1] << 4) | (values[i+2] >>2);
        if(i + 3 < len)
            decodedData[((i / 4) * 3)+2] = (values[i+2] << 6) | values[i+3];
    }
    return decodedLen;
}
