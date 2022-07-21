#pragma once //the compiler will only include this file once

#include "./../headers/typedefs.h" //Include typedef.h header (contains custom data types)
#include "./../headers/print.h"

struct MemoryMapEntry
{
    uint_64 BaseAddress;
    uint_64 RegionLength;
    uint_32 RegionType; //1 = RAM | 2 = RESERVED | 3 = ACPI Reclaimable | 4 = ACPI NVS | 5 = Bad Memory
    uint_32 ExtendedAttributes;
};

uint_8 useableMemoryRegionsCount = 0;

MemoryMapEntry** GetUseableMemoryRegions(); //Expose this method to our krnl
void PrintMemoryMap(MemoryMapEntry* memoryMap, uint_16 position); //Expose this method to our krnl