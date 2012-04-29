#include "Map.hpp"

#include "Utils.h"

#include "../tinyxml2/tinyxml2.h"

#include <cstring>
#include <zlib.h>

using namespace ore;
Map::Map() : mFilePath(""), mHeight(0), mWidth(0), mTileWidth(0),
mTileHeight(0), mTiles(NULL)
{
    //Nothing to do here. (yet?)
}

Map::~Map()
{
    delete [] mTiles;
}

int Map::Load(const char *file)
{

    //The .tmx map file
    tinyxml2::XMLDocument xmlFile;
    
    //Checking if the file opened without problems
    if (!xmlFile.LoadFile(file))
    {
        return ERR_FILE_READ_ERROR;
    }
    
    //Reading some basic info about this map
    xmlFile.RootElement()->Attribute("width", &mWidth);
    xmlFile.RootElement()->Attribute("height", &mHeight);
    xmlFile.RootElement()->Attribute("tilewidth", &mTileWidth);
    xmlFile.RootElement()->Attribute("tileheight", &mTileHeight);
    
    //Checking if all info is valid
    if (!(mWidth && mHeight && mTileHeight && mTileWidth))
        return ERR_INVALID_MAP_DATA;
    
    //Used to iterate through some nodes later
    tinyxml2::XMLNode *node;
        
    //Generic counter used for different things during the lading process
    int count = 0;
        
    //Loading the tileset
    if(node = xmlFile.RootElement()->FirstChild("tileset") == NULL)
        return ERR_INVALID_TILESET;

    for(mTilesetCnt = 0; mTilesetCnt < 256, ++mTilesetCnt)
    {
        
    }
    std::string tilesetPath = "../data/maps/";//TODO hardcoding, not good... Use data from a config file later.
    tilesetPath.append(node->FirstChildElement("image")->Attribute("source"));
    
    //Iterating through the tilesets, to be sure that there is only one tileset
    for (; node; node = node->NextSibling("tileset"))
        ++count;
                
    //Searching for the Ground layer
    node = xmlFile.RootElement()->FirstChild("layer");
    for (; node; node = node->NextSibling("tileset"))
    {
        if (strcmp(node->ToElement()->Attribute("name"), "Ground"))
            continue;
        break;
    }

    //Checking if there is data to read
    TiXmlString encoded("");
    encoded = node->FirstChild()->FirstChild()->Value();
    if (encoded.length() == 0)
        return ERR_INVALID_MAP_DATA;

    //Must be using zlib compression
    if (!node->FirstChildElement()->Attribute("compression"))
    return ERR_INVALID_COMPRESSION_METHOD;
    else if(strcmp(node->FirstChildElement()->Attribute("compression"), "zlib"))
    return ERR_INVALID_COMPRESSION_METHOD;


    //Rough approximation of the decoded size
    int maxSize = int(encoded.length() * .75) + 2;

    //Will store the decoded data
    unsigned char decoded[maxSize];

    //Decodes the data and returns its actual length
    int decodedLen = base64decode(encoded.c_str(), decoded, maxSize);

    //Now it's time to uncompress the it. The expected size of the uncompressed
    //data is 4 times the number of tiles in the map. 4 bytes (one int) per tile
    uLongf expectedSize = 4 * mHeight * mWidth;
    unsigned char uncompressed[expectedSize];
    int err;
    if((err = uncompress(uncompressed, &expectedSize, decoded, decodedLen)) < 0)
    {
        std::cerr << "zlib error code: " << err << std::endl;
        return ERR_ZLIB_ERROR; //Checking for errors during the uncompression
    }

    //If the size is no longer the same, something is wrong with the tile data
    if(expectedSize != 4 * mHeight * mWidth)
        return ERR_INVALID_TILE_DATA;

    //Converting (to int) and copying the tile data to the mTiles array
    mTiles = new int[mHeight * mWidth];
    for(int i = 0; i < expectedSize; i += 4)
    {
        mTiles[i / 4] = uncompressed[i] | (uncompressed[i + 1] << 8) |
        (uncompressed[i + 2] << 16) | uncompressed[i + 3] << 24;
        --mTiles[i /4]; // The tile count starts from 0, not 1
    }
    return 0;
}

int Map::AddTileset(const char* tilesetPath)
{
    
}

int Map::GetHeight() const
{
    return mHeight;
}

int Map::GetWidth() const
{
    return mWidth;
}

int Map::GetTileHeight() const
{
    return mTileHeight;
}

int Map::GetTileWidth() const
{
    return mTileWidth;
}

