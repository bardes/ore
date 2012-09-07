#include "Map.hpp"

#include "Base64.hpp"
#include "Utils.hpp"
#include "Layer.hpp"

#include "../tinyXml/tinyxml.h"

#include <cstring>
#include <iostream>
#include <string>
#include <zlib.h>

#include <sys/types.h>

ore::Map::Map() : mFilePath(""), mWidth(0), mHeight(0), mTileWidth(0),
mTileHeight(0), mPlayerLayer(0), mCached(false)
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

void ore::Map::Clear()
{
    mWidth = mHeight = mTileWidth = mTileHeight = mPlayerLayer = 0;
    mFilePath.clear();
    mLayerCacheBotom.clear();
    mLayerCacheTop.clear();
    mCached = false;

    for(int i = 0; i < mLayers.size(); ++i)
        delete mLayers[i];

    for(int i = 0; i < mTilesets.size(); ++i)
        delete mTilesets[i];
}

//TODO Make sure Load() does not mem leak if called twice or more
void ore::Map::Load(const std::string &file)
{
    //Clearing any old data
    Clear();

    //The .tmx map file
    TiXmlDocument xmlFile;

    //Checking if the file opened without problems
    if (!xmlFile.LoadFile(file.c_str()))
    {
        std::cerr << "Error: Could not read/find file: " << file << "\n"
        << "Maybe you don't have permission, or it's not a valid XML file.\n";
        throw ore::MAP_READ_ERROR;
    }

    //Reading some basic info about this map
    int Width, Height, TileHeight, TileWidth;
    Width = Height = TileHeight = TileWidth = 0;
    xmlFile.RootElement()->Attribute("width", &Width);
    xmlFile.RootElement()->Attribute("height", &Height);
    xmlFile.RootElement()->Attribute("tilewidth", &TileWidth);
    xmlFile.RootElement()->Attribute("tileheight", &TileHeight);

    //Checking if all properties are valid
    if(Width < 1 || Height < 1 || TileWidth < 1 || TileHeight < 1)
    {
        std::cerr << "Error: Invalid map properties here: " << file << "\n";
        throw ore::INVALID_MAP_DATA;
    }

    //Saving the data read from the map
    mWidth = Width;
    mHeight = Height;
    mTileWidth = TileWidth;
    mTileHeight = TileHeight;
    mFilePath = file;

    //Used to iterate through some xml elements later
    TiXmlElement *element;

    //Loading the tileset
    if((element = xmlFile.RootElement()->FirstChildElement("tileset")) == NULL)
    {
        std::cerr << "Error: Could not find any tileset on this map: "
        << file << "\n";
        throw ore::INVALID_TILESET;
    }

    //Loading the tilesets
    for(; element; element = element->NextSiblingElement("tileset"))
    {
        //Temporary data
        Tileset *tileset;
        int firstGid = 0;
        std::string path;

        //Reading the first gid
        element->Attribute("firstgid", &firstGid);
        if(firstGid == 0)
        {
            std::cerr<<"Error: Invalid first GID. (not found or equals to 0)\n";
            throw ore::INVALID_TILESET;
        }

        //Getting the path to the image file
        if(!element->FirstChildElement("image"))
        {
            std::cerr << "Error: Could not find <image> tag of tileset #" <<
            mTilesets.size() + 1 << " of this map: " << file << "\n";
            throw ore::INVALID_TILESET;
        }
        else if(!element->FirstChildElement("image")->Attribute("source"))
        {
            std::cerr << "Error: Could not find source attribute of tileset #"
            << mTilesets.size() + 1 << " of this map: " << file << "\n";
            throw ore::INVALID_TILESET;
        }
        else
        {
            path = element->FirstChildElement("image")->Attribute("source");
        }

        //Adding it
        AddTileset(path, firstGid, mTileWidth, mTileHeight);

    }

    //Searching for the first layer
    if((element = xmlFile.RootElement()->FirstChildElement("layer")) == NULL)
    {
        std::cerr << "Error: Map with no layers: " << file << "\n";
        throw ore::INVALID_MAP_DATA;
    }

    //Loading the layers
    for (int i = mLayers.size(); element && i < Max_Layers; ++i)
    {
        //Reading the name of the layer
        std::string name = "[unnamed layer]";
        if(element->Attribute("name"))
            name = element->Attribute("name");

        //Checking if there is a <data> tag, and if it's using the correct
        //compression method (zilb)
        if(!element->FirstChildElement("data"))
        {
            std::cerr << "Error: Could not find <data> tag of " << name
            << " of this map: " << file << "\n";
            throw ore::INVALID_MAP_DATA;
        }
        else if(!element->FirstChildElement("data")->GetText())
        {
            std::cerr << "Error: Could not read the data from <data> tag of "
            << name << " of this map: " << file << "\n";
            throw ore::INVALID_MAP_DATA;
        }
        else if(!element->FirstChildElement("data")->Attribute("compression"))
        {
            std::cerr << "Error: Could not identificate any compression method "
            "on this map: " << file << "\n";
            throw ore::INVALID_COMPRESSION_METHOD;
        }
        else if(strcmp(element->FirstChildElement("data")->
                       Attribute("compression"), "zlib"))
        {
            std::cerr << "Error: This map is not using zlib compression: "
            << file << "\n";
            throw ore::INVALID_COMPRESSION_METHOD;
        }

        std::string data = element->FirstChildElement("data")->GetText();
        AddLayer(data, mWidth, mHeight, name);

        //Go to the next layer in the file and iterate
        element = element->NextSiblingElement("layer");
    }

    //TODO Pre-render the layers
}

void ore::Map::AddTileset(std::string& path, ore::uint16 fGid,
                                       ore::uint8 tw, ore::uint8 th)
{
    //Checking if the new tileset doesn't conflicts with the old ones
    for(int i = 0; i < mTilesets.size(); ++i)
    {
        if(mTilesets[i]->IsMyGid(fGid))
        {
            std::cerr <<"Error: GID conflict! (Same GID found in different "
            "tilesets).\n";
            throw ore::TILESET_COLLISION;
        }
    }

    Tileset *loaded;

    try
    {
        loaded = new Tileset;
        loaded->Load(path, fGid, tw, th);
    }
    catch(ore::EXCEPTION exc)
    {
        delete loaded;
        throw exc;
    }
    catch(std::bad_alloc&)
    {
        std::cerr << "[tileset] Error: bad_alloc!\n";
        throw ore::BAD_ALLOC;
    }

    mTilesets.push_back(loaded);
}

void ore::Map::AddLayer(const std::string &data, ore::uint8 width,
                        ore::uint8 height, const std::string &name)
{
    //Decoding the base64 string
    std::vector<ore::uint8> decoded;
    base64decode(data, decoded);

    if(decoded.empty())
    {
        std::cerr << "Error: Corrupt data on layer #" << mLayers.size() + 1
        << " of this map:" << mFilePath << "\n";
        throw ore::INVALID_MAP_DATA;
    }

    //Now it's time to uncompress it. The expected size of the uncompressed
    //data is 4 times the number of tiles in the map. (one int per tile)
    uLongf expectedSize = 4 * width * height;
    ore::uint8 uncompressed[expectedSize];
    int err;
    if((err = uncompress(uncompressed, &expectedSize, decoded.data(),
        decoded.size())) < 0)
    {
        std::cerr << "Error: Corrupt data in " << name << " of this map: "
        << mFilePath << "\n(Zlib error code: " << err << ")\n";
        throw ore::ZLIB_ERROR;
    }

    //Zlib will change expectedSize if only part of the data is uncompressed
    //If expectedSize is no longer the same, something went wrong...
    if(expectedSize != 4 * width * height)
    {
        std::cerr << "Error: Corrupt data in " << name << " of this map:"
        << mFilePath << ".\n";
        throw ore::INVALID_MAP_DATA;
    }

    //Creating the new layer
    Layer *loaded;

    try
    {
    loaded = new Layer(width, height, name);
    }
    catch(std::bad_alloc&)
    {
        std::cerr << "[layer] Error: bad_alloc!\n";
        throw ore::BAD_ALLOC;
    }

    //Converting the data (into ints) and copying it to the layer
    for(int k = 0; k < expectedSize; k += 4)
    {
        (*loaded)[k / 4] = uncompressed[k] | (uncompressed[k + 1] << 8) |
        (uncompressed[k + 2] << 16) | uncompressed[k + 3] << 24;
    }

    mLayers.push_back(loaded);
}
