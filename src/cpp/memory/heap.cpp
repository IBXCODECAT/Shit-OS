#include "heap.h"

MemorySegmentHeader* firstFreeMemorySegment;

//A function to initialize our heap (the address won't change, but the length can during memory paging)
void InitHeap(uint_64 heapAddress, uint_64 heapLength)
{
    firstFreeMemorySegment = (MemorySegmentHeader*)heapAddress;
    firstFreeMemorySegment->MemoryLength = heapLength - sizeof(MemorySegmentHeader);

    //Initialize with null pointer references to previous and next segments
    firstFreeMemorySegment->NextSegment = 0;
    firstFreeMemorySegment->PreviousSegment = 0;
    firstFreeMemorySegment->NextFreeSegment = 0;
    firstFreeMemorySegment->PreviousFreeSegment = 0;

    firstFreeMemorySegment->free = true; //Initialize this memory segment as free
}

/// <summary>
/// allocate memory
/// <summary>
/// <param name=size>how much memory to allocate</param>
void* malloc(uint_64 size)
{
    //Here, for performance reasons, we align size by 8 bytes so that 64 bit processers can utilize memroy alignment
    uint_64 remainder = size % 8;
    size -= remainder;
    if(remainder != 0) size += 8;

    MemorySegmentHeader* currentMemorySegment = firstFreeMemorySegment;

    while(true)
    {
        //If the memory segment length is greater than or equal to the amount of memory requested...
        if(currentMemorySegment->MemoryLength >= size)
        {
            // Return the memory segment that's directly after the header which is free
            return currentMemorySegment + 1;
        }
    }
}