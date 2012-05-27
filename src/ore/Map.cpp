#include "Map.hpp"

#include "Utils.hpp"

#include "../tinyXml/tinyxml.h"

#include <cstring>
#include <iostream>
#include <string>
#include <zlib.h>

#include <sys/types.h>

ore::Map::Map() : mFilePath(""), mHeight(0), mWidth(0), mTileWidth(0),
mTileHeight(0), mLayerCount(0)
{
    for(ore::uint i = 0; i < Max_Layers; ++i)
        mLayers[i] = NULL;
}

ore::Map::~Map()
{
    for(ore::uint i = 0; i < Max_Layers; ++i)
        delete [] mLayers[i];
    
    for(ore::uint i = 0; i < mTilesets.size(); ++i)
    {
        delete *mTilesets.begin();
        mTilesets.pop_front();
    }
}

//TODO Make sure Load() does not mem leak if called twice or more
ore::RETURN_VALUE ore::Map::Load(const std::string &file)
{

    //The .tmx map file
    TiXmlDocument xmlFile;
    
    //Checking if the file opened without problems
    if (!xmlFile.LoadFile(file.c_str()))
    {
        std::cerr << "Error: Could not read/open file: " << file << "\n"
        << "Maybe you don't have permission, or it's not a valid XML file.\n";
        return ore::ERR_MAP_READ_ERROR;
    }
    
    //Reading some basic info about this map
    int Width, Height, TileHeight, TileWidth;
    Width = Height = TileHeight = TileWidth = 0;
    xmlFile.RootElement()->Attribute("width", &Width);
    xmlFile.RootElement()->Attribute("height", &Height);
    xmlFile.RootElement()->Attribute("tilewidth", &TileWidth);
    xmlFile.RootElement()->Attribute("tileheight", &TileHeight);

    //Saving the data read from the map
    mWidth = Width;
    mHeight = Height;
    mTileWidth = TileWidth;
    mTileHeight = TileHeight;
    
    //Checking if all properties are valid
    if(Width < 1 || Height < 1 || TileWidth < 1 || TileHeight < 1)
    {
        std::cerr << "Error: Invalid map properties here: " << file << "\n";
        return ore::ERR_MAP_INVALID_DATA;
    }
    
    //Used to iterate through some xml elements later
    TiXmlElement *element;
        
    //Generic counter used for different things during the lading process
    int count = 0;
    
    //Loading the tileset
    if((element = xmlFile.RootElement()->FirstChildElement("tileset")) == NULL)
    {
        std::cerr << "Error: Could not find any tileset on this map: "
        << file << "\n";
        return ore::ERR_MAP_INVALID_TILESET;
    }
    
    //Getting the directory path of this map
    ore::uint slashPos;
    std::string mapDir;
    for(slashPos = 1; slashPos < file.length(); ++slashPos)
    {
        if((*(file.end() - slashPos)) == '/')
            break;
    }
    //There may be no slashes at all, which means the current working directory is
    //the map dir. In this case nothing needs to be added to the tileset path
    if(slashPos < file.length())
    {
        //Erasing the file name from the end of the path
        //The +1 in the end is to keep the slash ("/") at the end of the path
        mapDir.assign(file, 0, file.length() - slashPos + 1);
    }
    
    //Loading the tilesets
    //TODO Separate this and make an addTileset() function
    for(; element; element = element->NextSiblingElement("tileset"))
    {
        //Temporary data
        Tileset *tileset;
        int firstGid = 0;
        std::string path;
        ore::RETURN_VALUE retVal;

        //Reading the first gid
        element->Attribute("firstgid", &firstGid);
        if(firstGid == 0)
        {
            std::cerr << "First GID can not be 0!";
            return ore::ERR_MAP_INVALID_TILESET;
        }

        //Getting the path to the image file
        if(!element->FirstChildElement("image"))
        {
            std::cerr << "Error: Could not find <image> tag of tileset #" <<
            mTilesets.size() + 1 << " of this map: " << file << "\n";
            return ore::ERR_MAP_INVALID_TILESET;
        }
        else if(!element->FirstChildElement("image")->Attribute("source"))
        {
            std::cerr << "Error: Could not find source of tileset #" <<
            mTilesets.size() + 1 << " of this map: " << file << "\n";
            return ore::ERR_MAP_INVALID_TILESET;
        }
        else
        {
            path = element->FirstChildElement("image")->Attribute("source");
        }
        
        //Creating a new tileset
        tileset = new Tileset;

        //Loading the tileset
        if((retVal = tileset->Load(mapDir + path, firstGid,
                                 mTileWidth, mTileHeight)) != ore::SUCCESS)
        {
            delete tileset;
            std::cerr << "Error: Could not load tileset #" << mTilesets.size() + 1
            << " of this map: " << file << "\n";
            return retVal;
        }
        mTilesets.push_back(tileset);
    }

    //Searching for the first layer
    if((element = xmlFile.RootElement()->FirstChildElement("layer")) == NULL)
    {
        std::cerr << "Error: Map with no layers: " << file << "\n";
        return ore::ERR_MAP_INVALID_DATA;
    }
    
    //Loading the layers
    //TODO Separate this, and make an addLayer() function
    //TODO Read the layer name and check for a * character to define mPlayerLayer
    for (ore::uint i = 0; element && i < Max_Layers; ++i)
    {
        //Checking if there is a <data> tag, and if it's using the correct
        //compression method (zilb)
        if(!element->FirstChildElement("data"))
        {
            std::cerr << "Error: Could not find <data> tag of layer #" << i + 1 <<
            " of this map: " << file << "\n";
            return ore::ERR_MAP_INVALID_DATA;
        }
        else if(!element->FirstChildElement("data")->GetText())
        {
            std::cerr << "Error: Could not read the data from <data> tag of layer"
            " #" << i + 1 << " of this map: " << file << "\n";
            return ore::ERR_MAP_INVALID_DATA;
        }
        else if(!element->FirstChildElement("data")->Attribute("compression"))
        {
            std::cerr << "Error: Could not identificate any compression method on"
            " this map: " << file << "\n";
            return ore::ERR_MAP_INVALID_COMPRESSION_METHOD;
        }
        else if(strcmp(element->FirstChildElement("data")->
                       Attribute("compression"), "zlib"))
        {
            std::cerr << "Error: This map is not using zlib compression: "
            << file << "\n";
            return ore::ERR_MAP_INVALID_COMPRESSION_METHOD;
        }
        
        //Decoding the base64 string
        std::string data = element->FirstChildElement("data")->GetText();
        std::vector<ore::byte> decoded;
        base64decode(data, decoded);

        if(decoded.empty())
        {
            std::cerr << "Error: Corrupt data on layer #" << i+1<< " of this map:"
            << file << "\n";
            return ore::ERR_MAP_INVALID_DATA;
        }

        //Now it's time to uncompress it. The expected size of the uncompressed
        //data is 4 times the number of tiles in the map. (one int per tile)
        uLongf expectedSize = 4 * mHeight * mWidth;
        ore::byte uncompressed[expectedSize];
        int err;
        if((err = uncompress(uncompressed, &expectedSize, decoded.data(),
            decoded.size())) < 0)
        {
            std::cerr << "Error: Corrupt data on layer #" << i+1<< " of this map:"
            << file << "\n";
            std::cerr << "(Zlib error code: " << err << ")\n";
            return ore::ERR_MAP_ZLIB_ERROR;
        }

        //Zlib will change expectedSize if it can only uncompress part of the data
        //If expectedSize is no longer the same, something is wrong with the data
        if(expectedSize != 4 * mHeight * mWidth)
        {
            std::cerr << "Error: Corrupt data on layer #" << i+1<< " of this map:"
            << file << "\n";
            return ore::ERR_MAP_INVALID_DATA;
        }

        //Converting (to int) and copying the layers data to the mLayers array
        mLayers[i] = new ore::uint[mHeight * mWidth];
        for(int k = 0; k < expectedSize; k += 4)
        {
            mLayers[i][k / 4] = uncompressed[k] | (uncompressed[k + 1] << 8) |
            (uncompressed[k + 2] << 16) | uncompressed[k + 3] << 24;
        }
        
        //Go to the next layer in the file and iterate
        element = element->NextSiblingElement("layer");
        ++mLayerCount;
    }
    
    //TODO Pre-rendering the layers
    
    return ore::SUCCESS;
}