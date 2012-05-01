#include "Map.hpp"

#include "Utils.hpp"

#include "../tinyXml/tinyxml.h"

#include <cstring>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <zlib.h>

ore::Map::Map() : mFilePath(""), mHeight(0), mWidth(0), mTileWidth(0),
mTileHeight(0)
{
    for(uint i = 0; i < Max_Layes; ++i)
        mLayers[i] = NULL;
}

ore::Map::~Map()
{
    for(uint i = 0; i < Max_Layes; ++i)
        delete mLayers[i];
}

int ore::Map::Load(const std::string &file)
{

    //The .tmx map file
    TiXmlDocument xmlFile;
    
    //Checking if the file opened without problems
    if (!xmlFile.LoadFile(file.c_str()))
    {
        return ERR_FILE_READ_ERROR;
    }
    
    //Reading some basic info about this map
    int Width, Height, TileHeight, TileWidth;
    Width = Height = TileHeight = TileWidth = 0;
    xmlFile.RootElement()->Attribute("width", &Width);
    xmlFile.RootElement()->Attribute("height", &Height);
    xmlFile.RootElement()->Attribute("tilewidth", &TileWidth);
    xmlFile.RootElement()->Attribute("tileheight", &TileHeight);

    mWidth = Width;
    mHeight = Height;
    mTileWidth = TileWidth;
    mTileHeight = TileHeight;
    
    //Checking if all info is valid
    if (!(mWidth && mHeight && mTileHeight && mTileWidth))
        return ERR_INVALID_MAP_DATA;
    
    //Used to iterate through some nodes later
    TiXmlNode *node;
        
    //Generic counter used for different things during the lading process
    int count = 0;
    //Loading the tileset
    if((node = xmlFile.RootElement()->FirstChildElement("tileset")) == NULL)
        return ERR_INVALID_TILESET;

//     //TODO<Use data from a config file later> (not hard-coding)
//     std::string defaultTilesetDir = "../data/maps/";
//     //TODO<Clear tilesets here> //In case of reloading a map
//     for(; node; node = node->NextSiblingElement("tileset"))
//     {
//     //TODO<Load the tilesets>
//     }

    //TODO Do the SDL part of loading tile images and so on

    //Searching for the first layer
    if((node = xmlFile.RootElement()->FirstChildElement("layer")) == NULL)
        return ERR_INVALID_MAP_DATA;

    for (uint i = 0; node && i < Max_Layes; ++i)
    {
        //Must be using zlib compression
        if (!node->FirstChildElement("data")->Attribute("compression"))
            return ERR_INVALID_COMPRESSION_METHOD;
        else if(strcmp(node->FirstChildElement("data")->
                       Attribute("compression"), "zlib"))
            return ERR_INVALID_COMPRESSION_METHOD;

        //Decoding the base64 string
        std::string data = node->FirstChildElement("data")->GetText();
        std::vector<Byte> decoded;
        base64decode(data, decoded);

        if(decoded.empty())
            return ERR_INVALID_MAP_DATA;

        //Now it's time to uncompress it. The expected size of the uncompressed
        //data is 4 times the number of tiles in the map. 4 bytes (one int) per tile
        ulong expectedSize = 4 * mHeight * mWidth;
        Byte uncompressed[expectedSize];
        int err;
        if((err = uncompress(uncompressed, &expectedSize, decoded.data(),
            decoded.size())) < 0)
        {
            std::cerr << "zlib error code: " << err << std::endl;
            return ERR_ZLIB_ERROR;
        }

        //If the size is no longer the same, something is wrong with the data
        if(expectedSize != 4 * mHeight * mWidth)
            return ERR_INVALID_TILE_DATA;

        //Converting (to int) and copying the layers data to the mLayers array
        mLayers[i] = new uint[mHeight * mWidth];
        for(int k = 0; k < expectedSize; k += 4)
        {
            mLayers[i][k / 4] = uncompressed[k] | (uncompressed[k + 1] << 8) |
            (uncompressed[k + 2] << 16) | uncompressed[k + 3] << 24;
        }
        
        //Go to the next layer in the file
        node = node->NextSiblingElement("layer");
    }

//      //Debug
//     for(int i = 0; i < Max_Layes; ++i)
//     {
//         if(mLayers[i])
//         {
//             std::cout << "Layer " << i << ":\n";
//             for(int j = 0; j < mHeight * mWidth; ++j)
//             {
//                 std::cout << j << "\t - " << mLayers[i][j] << '\n';
//             }
//         }
//         else
//         {
//             std::cout << "Layer " << i << ": NULL\n";
//         }
//     }
    return SUCCESS;
}

int ore::Map::GetHeight() const
{
    return mHeight;
}

int ore::Map::GetWidth() const
{
    return mWidth;
}

int ore::Map::GetTileHeight() const
{
    return mTileHeight;
}

int ore::Map::GetTileWidth() const
{
    return mTileWidth;
}
