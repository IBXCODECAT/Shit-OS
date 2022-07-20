#pragma once //the compiler will only include this file once

#include "./../headers/typedefs.h"

struct MemorySegmentHeader
{
    uint_64 MemoryLength; //The length for this segment

    MemorySegmentHeader* NextSegment; //A pointer to the next memory segment header
    MemorySegmentHeader* PreviousSegment; //A pointer tot the previous memroy segment header

    MemorySegmentHeader* NextFreeSegment; //A pointer to the next free segment header
    MemorySegmentHeader* PreviousFreeSegment; //A pointer to the previous free segment header

    bool free; //is this memory region free?
};

extern void InitHeap(uint_64 heapAddress, uint_64 heapLength); //Extern allows this to be called from our kernel
extern void* malloc(uint_64 size); //Extern allows this to be called from our kernel