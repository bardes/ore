#include "Map.hpp"

#include "Utils.hpp"
#include "Layer.hpp"

#include "../tinyXml/tinyxml.h"

#include <cstring>
#include <iostream>
#include <string>
#include <zlib.h>

#include <sys/types.h>

ore::Map::Map() : mFilePath(""), mWidth(0), mHeight(0), mTileWidth(0),
mTileHeight(0)
{
    //Reserving some space to avoid reallocations
    mLayers.reserve(32);
    mTilesets.reserve(32);
}

ore::Map::~Map()
{
    for(int i = 0; i < mLayers.size(); ++i)
        delete mLayers[i];

    for(int i = 0; i < mTilesets.size(); ++i)
        delete mTilesets[i];
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
    int slashPos;
    std::string mapDir;
    for(slashPos = 1; slashPos < file.length(); ++slashPos)
    {
        if((*(file.end() - slashPos)) == '/')
            break;
    }
    //There may be no slashes at all, which means the current working directory
    //is the map dir. In this case nothing needs to be added to the tileset path
    if(slashPos < file.length())
    {
        //Erasing the file name from the end of the path
        //The +1 in the end is to keep the slash ("/") at the end of the path
        mapDir.assign(file, 0, file.length() - slashPos + 1);
    }

    //Loading the tilesets
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
            std::cerr << "Error: First GID can not be 0!";
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
            std::cerr << "Error: Could not find source attribute of tileset #"
            << mTilesets.size() + 1 << " of this map: " << file << "\n";
            return ore::ERR_MAP_INVALID_TILESET;
        }
        else
        {
            path = element->FirstChildElement("image")->Attribute("source");
        }

        //Adding it
        if((retVal = AddTileset(path, firstGid, mTileWidth, mTileHeight))
            != ore::SUCCESS)
        {
            std::cerr << "Error: Could not load tileset #" << mTilesets.size()+1
            << " of this map: " << file << "\n";
            return retVal;
        }
    }

    //Searching for the first layer
    if((element = xmlFile.RootElement()->FirstChildElement("layer")) == NULL)
    {
        std::cerr << "Error: Map with no layers: " << file << "\n";
        return ore::ERR_MAP_INVALID_DATA;
    }

    //Loading the layers
    //TODO Read the layer name and check for a * character to define mPlayerLayer
    //TODO a lot of things
    for (int i = 0; element && i < Max_Layers; ++i)
    {
        //Checking if there is a <data> tag, and if it's using the correct
        //compression method (zilb)
        if(!element->FirstChildElement("data"))
        {
            std::cerr << "Error: Could not find <data> tag of layer #" << i + 1
            << " of this map: " << file << "\n";
            return ore::ERR_MAP_INVALID_DATA;
        }
        else if(!element->FirstChildElement("data")->GetText())
        {
            std::cerr << "Error: Could not read the data from <data> tag of "
            "layer #" << i + 1 << " of this map: " << file << "\n";
            return ore::ERR_MAP_INVALID_DATA;
        }
        else if(!element->FirstChildElement("data")->Attribute("compression"))
        {
            std::cerr << "Error: Could not identificate any compression method "
            "on this map: " << file << "\n";
            return ore::ERR_MAP_INVALID_COMPRESSION_METHOD;
        }
        else if(strcmp(element->FirstChildElement("data")->
                       Attribute("compression"), "zlib"))
        {
            std::cerr << "Error: This map is not using zlib compression: "
            << file << "\n";
            return ore::ERR_MAP_INVALID_COMPRESSION_METHOD;
        }

        //TODO Use the layer dimensions instead of the map dimensions
        std::string data = element->FirstChildElement("data")->GetText();
        AddLayer(data, mWidth, mHeight);

        //Go to the next layer in the file and iterate
        element = element->NextSiblingElement("layer");
    }

    //TODO Pre-rendering the layers

    return ore::SUCCESS;
}

ore::RETURN_VALUE ore::Map::AddTileset(std::string& path, ore::uint16 fGid,
                                       ore::uint8 tw, ore::uint8 th)
{
    Tileset *loaded = new Tileset;
    ore::RETURN_VALUE ret;
    ret = loaded->Load(path, fGid, tw, th);

    if(ret != ore::SUCCESS)
    {
        delete loaded;
        return ret;
    }

    mTilesets.push_back(loaded);
    return ore::SUCCESS;
}

void ore::Map::AddLayer(const std::string &data, ore::uint8 width,
                        ore::uint8 height)
{
    //Decoding the base64 string
    std::vector<ore::uint8> decoded;
    base64decode(data, decoded);

    if(decoded.empty())
    {
//         std::cerr << "Error: Corrupt data on layer #" << i + 1
//         << " of this map:" << file << "\n";
//         return ore::ERR_MAP_INVALID_DATA;
    }

    //Now it's time to uncompress it. The expected size of the uncompressed
    //data is 4 times the number of tiles in the map. (one int per tile)
    uLongf expectedSize = 4 * width * height;
    ore::uint8 uncompressed[expectedSize];
    int err;
    if((err = uncompress(uncompressed, &expectedSize, decoded.data(),
        decoded.size())) < 0)
    {
//         std::cerr << "Error: Corrupt data on layer #" << i + 1
//         << " of this map:" << file << "\n";
//         std::cerr << "(Zlib error code: " << err << ")\n";
//         return ore::ERR_MAP_ZLIB_ERROR;
    }

    //Zlib will change expectedSize if only part of the data is uncompressed
    //If expectedSize is no longer the same, something went wrong...
    if(expectedSize != 4 * width * height)
    {
//         std::cerr << "Error: Corrupt data on layer #" << i + 1
//         << " of this map:" << file << "\n";
//         return ore::ERR_MAP_INVALID_DATA;
    }

    //Converting the data (to ints) and copying it to the layer
    Layer *loaded = new Layer(width, height);
    for(int k = 0; k < expectedSize; k += 4)
    {
        (*(loaded))[k / 4] = uncompressed[k] | (uncompressed[k + 1] << 8) |
        (uncompressed[k + 2] << 16) | uncompressed[k + 3] << 24;
    }

    mLayers.push_back(loaded);
}
