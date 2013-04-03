#include "Map.hpp"

#include "Base64.hpp"
#include "Layer.hpp"
#include "Tileset.hpp"
#include "Utils.hpp"

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
    //Don't panic!
    //The layers and tilesets will be deleted by the resource manager
}

void ore::Map::Clear()
{
    mWidth = mHeight = mTileWidth = mTileHeight = 0;
    mFilePath.clear();
    mTilesets.clear();
    mLayers.clear();
    mLocalMgr.Clear();
}

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

    //Used to iterate through some xml elements...
    TiXmlElement *element;

    //Looking for the tileset
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
        ore::Tileset *tileset;
        int firstGid = 0;
        int tw, th;
        tw = th = 0;
        std::string path = file;

        //Removing the file name
        while((*path.rbegin()) != '/' && path.end() != path.begin())
            path.resize(path.size() - 1);

        //Reading the first gid
        element->Attribute("firstgid", &firstGid);
        if(firstGid == 0)
        {
            std::cerr<<"Error: Invalid first GID. (not found or equals to 0)\n";
            throw ore::INVALID_TILESET;
        }

        //Reading tile width and height
        element->Attribute("tilewidth", &tw);
        if(firstGid == 0)
        {
            std:: cerr << "Error: Can't read tile widht. (not found or equals to 0)\n";
            throw ore::INVALID_TILESET;
        }
        element->Attribute("tileheight", &th);
        if(firstGid == 0)
        {
            std:: cerr << "Error: Can't read tile height. (not found or equals to 0)\n";
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
            path.append(element->FirstChildElement("image")->Attribute("source"));
        }

        AddTileset(path, firstGid, tw, th);

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

        //Checking if there is a <data>
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
            std::cerr << "Error: Could not identify any compression method "
            "on this map: " << file << "\n";
            throw ore::INVALID_COMPRESSION_METHOD;
        }
        else if(strcmp(element->FirstChildElement("data")->Attribute("compression"), "zlib") &&
                strcmp(element->FirstChildElement("data")->Attribute("compression"), "gzip"))
        {
            std::cerr << "Error: This map is not using a valid compression method: "
            << file << "\n";
            throw ore::INVALID_COMPRESSION_METHOD;
        }

        std::string data = element->FirstChildElement("data")->GetText();
        AddLayer(data, name);
        element = element->NextSiblingElement("layer");
    }

    RenderCache();
}

void ore::Map::RenderCache()
{
    if(mLayers.size() == 0)
        return;

    for(int i = 0; i < mLayers.size(); ++i)
        mLayers[i]->RenderCache(mTilesets);
}

sf::Sprite ore::Map::GetLayerImg(ore::uint16 layer)
{
    if(layer >= mLayers.size())
    {
        std::cerr << "Warning: Layer out of range!\n";
        return sf::Sprite();
    }

    return mLayers[layer]->GetImage();
}

void ore::Map::AddTileset(Tileset *newTileset, bool reg)
{
    //Checking if the new tileset doesn't conflicts with the old ones
    for(int i = 0; i < mTilesets.size(); ++i)
    {
        if(mTilesets[i]->IsMyGid(newTileset->GetFirstGid()))
        {
            std::cerr <<"Error: GID conflict! (Same GID found in different "
            "tilesets).\n";
            throw ore::TILESET_COLLISION;
        }
    }

    newTileset->AddUser(this);
    mTilesets.push_back(newTileset);
    if(reg)
        mLocalMgr.Register(newTileset);
}

ore::Tileset* ore::Map::AddTileset(const std::string &path, ore::uint16 fGid,
                                       ore::uint8 tw, ore::uint8 th, ResourceManager* mgr)
{
    //Checking if the new tileset doesn't conflicts with the old ones
    for(int i = 0; i < mTilesets.size(); ++i)
    {
        if(mTilesets[i]->IsMyGid(fGid))
        {
            std::cerr <<"Error: GID conflict! (Same GID found in different "
                        "tilesets).\n" << "path: " << path << "\n";
            throw ore::TILESET_COLLISION;
        }
    }

    ore::Tileset *loaded;

    try
    {
        loaded = new ore::Tileset;
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

    loaded->AddUser(this);
    mTilesets.push_back(loaded);

    if(mgr == NULL)
        mLocalMgr.Register(loaded);
    else
        mgr->Register(loaded);

    return loaded;
}

bool ore::Map::DeleteTileset(Tileset* tileset)
{
    for(int i = 0; i < mTilesets.size(); ++i)
    {
        if(mTilesets[i] == tileset)
        {
            mTilesets[i]->DeleteUser(this);
            mTilesets.erase(mTilesets.begin() + i);
            return false;
        }
    }
    return true;
}

//TODO Load uncompressed data too
ore::Layer* ore::Map::AddLayer(const std::string &data, const std::string &name, int pos)
{
    //Chacking if the map has the necessary data.
    if(!(mWidth && mHeight && mTileWidth && mTileHeight))
        return NULL;

    //Decoding the base64 string
    std::vector<ore::uint8> decoded;
    base64decode(data, decoded);

    if(decoded.empty())
    {
        std::cerr << "Error: Corrupt data on layer #" << mLayers.size() + 1
        << " of this map:" << mFilePath << "\n";
        throw ore::INVALID_MAP_DATA;
    }

    //Now it's time to uncompress it.
    std::vector<ore::uint8> uncompressed;
    if(ore::Uncompress(decoded, uncompressed, 4 * mWidth * mHeight))
    {
        std::cerr << "Error: Corrupt data in " << name << " of this map: "
        << mFilePath << ".\n";
        throw ore::INVALID_MAP_DATA;
    }

    //Creating the new layer
    Layer *loaded;
    try
    {
        loaded = new Layer(mWidth, mHeight, mTileWidth, mTileHeight, name);
    }
    catch(std::bad_alloc&)
    {
        std::cerr << "[layer] Error: bad_alloc!\n";
        throw ore::BAD_ALLOC;
    }

    //Converting the data (into ints) and copying it to the layer
    for(int k = 0; k < uncompressed.size(); k += 4)
    {
        loaded->AddGid(uncompressed[k] | (uncompressed[k + 1] << 8) |
        (uncompressed[k + 2] << 16) | uncompressed[k + 3] << 24);
    }

    loaded->AddUser(this);
    mLocalMgr.Register(loaded);

    if(pos < 0 || pos >= mLayers.size())
        mLayers.push_back(loaded);
    else
        mLayers.insert(mLayers.begin() + pos, loaded);

    return loaded;
}

void ore::Map::AddLayer(Layer *newLayer, int pos)
{
    if(mLayers.size() >= Max_Layers)
        return;

    newLayer->AddUser(this);
    mLocalMgr.Register(newLayer);

    if(pos < 0 || pos >= mLayers.size())
        mLayers.push_back(newLayer);
    else
        mLayers.insert(mLayers.begin() + pos, newLayer);
}
