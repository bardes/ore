#include "Utils.hpp"

#include <zlib.h>
#include <iostream>

int ore::Uncompress(const std::vector<ore::uint8>& data, std::vector<ore::uint8>& uncompressed,
                    ore::uint32 expectedSize)
{
    uncompressed.resize(expectedSize);

    z_stream s;
    s.opaque = Z_NULL;
    s.zalloc = Z_NULL;
    s.zfree = Z_NULL;
    s.next_in = (Bytef *) data.data();
    s.avail_in = uncompressed.size();
    s.next_out = (Bytef *) uncompressed.data();
    s.avail_out = expectedSize;

    int err = inflateInit2(&s, 15 + 32);
    if(err != Z_OK)
        return err;

    do
    {
        err = inflate(&s, Z_SYNC_FLUSH);

        if(err == Z_MEM_ERROR)
        {
            std::cerr << "Warning: The uncompressed data is bigger than expected!\n";
            int oldSize = uncompressed.size();
            uncompressed.resize(oldSize * 2);
            s.next_out = uncompressed.data() + oldSize;
            s.avail_out = oldSize;
            continue;
        }
        else if(err != Z_OK && err != Z_STREAM_END)
        {
            inflateEnd(&s);
            std::cerr << "Error: Could not uncompress the data!\n";
            uncompressed.clear();
            return err;
        }
    }
    while(err != Z_STREAM_END);

    if(s.avail_out > 0)
        std::cerr << "Warning: The uncompressed data is smaller than expected!\n";

    return 0;
}
