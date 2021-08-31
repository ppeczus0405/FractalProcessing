#ifndef BITMAP_FILE_HEADER
#define BITMAP_FILE_HEADER

#include <cstdint>
using namespace std;
 
#pragma pack(push, 2)
struct BitmapFileHeader
{
    const char header[2] = {'B', 'M'};
    int32_t fileSize;
    int32_t reserved = 0;
    int32_t dataOffset;
};
#pragma pack(pop)

#endif // BITMAP_FILE_HEADER