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

extern uint_8 MemoryRegionCount;
uint_8 useableMemoryRegionsCount = 0;

MemoryMapEntry* useableMemoryRegions[10];

bool gotUseableMemoryRegions = false;

MemoryMapEntry** GetUseableMemoryRegions()
{
    if(gotUseableMemoryRegions)
    {
        return useableMemoryRegions;
    }

    uint_8 UseableRegionIndex;
    for(uint_8 i = 0; i < MemoryRegionCount; i++)
    {
        MemoryMapEntry* memMap = (MemoryMapEntry*)0x5000;
        memMap += i;
        if(memMap->RegionType == 1)
        {
            useableMemoryRegions[UseableRegionIndex] = memMap;
            UseableRegionIndex++;
        }
        
        useableMemoryRegionsCount = UseableRegionIndex;
    }

    gotUseableMemoryRegions = true;
    return useableMemoryRegions;
} 

void PrintMemoryMap(MemoryMapEntry* memoryMap, uint_16 position)
{
    SetCursorPosition(position);
    PrintString("Memory Base: "); PrintString(IntegerToString(memoryMap->BaseAddress));
    SetCursorPosition(position + VGA_WIDTH);
    
    PrintString("Memory Region Length: "); PrintString(IntegerToString(memoryMap->RegionLength));
    SetCursorPosition(position + (VGA_WIDTH * 2));

    PrintString("Memory Region Type: "); PrintString(IntegerToString(memoryMap->RegionType));
    SetCursorPosition(position + (VGA_WIDTH * 3));

    PrintString("Memory Attributes: "); PrintString(HexToString(memoryMap->ExtendedAttributes));
    SetCursorPosition(position + (VGA_WIDTH * 4));
}

