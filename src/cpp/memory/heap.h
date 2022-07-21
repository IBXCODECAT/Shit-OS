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

void InitHeap(uint_64 heapAddress, uint_64 heapLength); //Forward declare this function from source so it can be acessed in the krnl
void* malloc(uint_64 size); //Forward declare this function from source so it can be acessed in the krnl
void free(void* address); //Forward declare this function from source so it can be acessed in the krnl