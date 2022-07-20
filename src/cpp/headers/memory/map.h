#pragma once //the compiler will only include this file once

#include "./../typedefs.h" //Include typedef.h header (contains custom data types)

struct MemoryMapEntry
{
    uint_64 BaseAddress;
    uint_64 RegionLength;
    uint_32 RegionType;
    uint_32 ExtendedAttributes;
};

extern uint_8 MemoryRegionCount;




