#include "Map.hpp"

#include "Utils.hpp"

#include "../tinyXml/tinyxml.h"

#include <cstring>
#include <iostream>
#include <string>
#include <zlib.h>

#include <sys/types.h>

ore::Map::Map() : mFilePath(""), mHeight(0), mWidth(0), mTileWidth(0),
mTileHeight(0)
{
    for(ore::uint i = 0; i < Max_Layers; ++i)
        mLayers[i] = NULL;
}

ore::Map::~Map()
{
    for(ore::uint i = 0; i < Max_Layers; ++i)
        delete [] mLayers[i];
}

//TODO Add error messages!
int ore::Map::Load(const std::string &file)
{

    //The .tmx map file
    TiXmlDocument xmlFile;
    
    //Checking if the file opened without problems
    if (!xmlFile.LoadFile(file.c_str()))
    {
        return ore::ERR_MAP_READ_ERROR;
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
        return ore::ERR_INVALID_MAP_DATA;
    
    //Used to iterate through some xml elements later
    TiXmlElement *element;
        
    //Generic counter used for different things during the lading process
    int count = 0;
    
    //Loading the tileset
    if((element = xmlFile.RootElement()->FirstChildElement("tileset")) == NULL)
        return ore::ERR_INVALID_TILESET;

    //TODO Use data from a config file later (not hard-coding)
    std::string defaultTilesetDir = "./";
    mTilesets.clear();//In case of reloading a map
    for(; element; element = element->NextSiblingElement("tileset"))
    {
        //Temporary data
        Tileset tmp;
        int firstGid = 0;
        std::string path;
        int err;

        //Reading the first gid
        element->Attribute("firstgid", &firstGid);
        if(firstGid == 0)
        {
            std::cerr << "First GID can not be 0!";
            return ore::ERR_INVALID_TILESET;
        }

        //Getting the path to the image file
        if(element->FirstChildElement("image"))
            path = element->FirstChildElement("image")->Attribute("source");
        else
            return ore::ERR_INVALID_TILESET;

        //Loading the tileset (I think there is a smarter way of doing this...)
        if((err = tmp.Load(defaultTilesetDir + path, firstGid,
                                 mTileWidth, mTileHeight)) != ore::SUCCESS)
        {
            return err;
        }
        mTilesets.push_back(tmp);
    }

    //Searching for the first layer
    if((element = xmlFile.RootElement()->FirstChildElement("layer")) == NULL)
        return ore::ERR_INVALID_MAP_DATA;

    for (ore::uint i = 0; element && i < Max_Layers; ++i)
    {
        //Must be using zlib compression
        if (!element->FirstChildElement("data")->Attribute("compression"))
            return ore::ERR_INVALID_COMPRESSION_METHOD;
        else if(strcmp(element->FirstChildElement("data")->
                       Attribute("compression"), "zlib"))
            return ore::ERR_INVALID_COMPRESSION_METHOD;

        //Decoding the base64 string
        //TODO Check for a NULL string
        std::string data = element->FirstChildElement("data")->GetText();
        std::vector<ore::byte> decoded;
        base64decode(data, decoded);

        if(decoded.empty())
            return ore::ERR_INVALID_MAP_DATA;

        //Now it's time to uncompress it. The expected size of the uncompressed
        //data is 4 times the number of tiles in the map. (one int per tile)
        uLongf expectedSize = 4 * mHeight * mWidth;
        ore::byte uncompressed[expectedSize];
        int err;
        if((err = uncompress(uncompressed, &expectedSize, decoded.data(),
            decoded.size())) < 0)
        {
            std::cerr << "zlib error code: " << err << std::endl;
            return ore::ERR_ZLIB_ERROR;
        }

        //If the size is no longer the same, something is wrong with the data
        if(expectedSize != 4 * mHeight * mWidth)
            return ore::ERR_INVALID_MAP_DATA;

        //Converting (to int) and copying the layers data to the mLayers array
        mLayers[i] = new ore::uint[mHeight * mWidth];
        for(int k = 0; k < expectedSize; k += 4)
        {
            mLayers[i][k / 4] = uncompressed[k] | (uncompressed[k + 1] << 8) |
            (uncompressed[k + 2] << 16) | uncompressed[k + 3] << 24;
        }
        
        //Go to the next layer in the file
        element = element->NextSiblingElement("layer");
    }

    //TODO Pre render the layers

//     //Debug
//     for(int i = 0; i < Max_Layers; ++i)
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
    return ore::SUCCESS;
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
