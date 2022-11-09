#pragma once //the compiler will only include this file once

#include "./../headers/typedefs.h"
#include "memory.h"

struct MemorySegmentHeader
{
    uint_64 MemoryLength; //The length for this segment

    MemorySegmentHeader* NextSegment; //A pointer to the next memory segment header
    MemorySegmentHeader* PreviousSegment; //A pointer tot the previous memroy segment header

    MemorySegmentHeader* NextFreeSegment; //A pointer to the next free segment header
    MemorySegmentHeader* PreviousFreeSegment; //A pointer to the previous free segment header

    bool free; //is this memory region free?
};

struct AlignedMemorySegmentHeader
{
    uint_64 MemmorySegmentHeaderAddress : 63;

    //Set the significant bit to one so the free() function can recognize what an aligned memory segment header is
    //In a normal segment header it will not be one because we will not be working with bytes in the trillions 
    bool isAligned : 1; 
};

void InitHeap(uint_64 heapAddress, uint_64 heapLength); //Forward declare this function from source so it can be acessed in the krnl
void* malloc(uint_64 allocSize); //Forward declare this function from source so it can be acessed in the krnl
void free(void* address); //Forward declare this function from source so it can be acessed in the krnl
void* calloc(uint_64 allocSize); //Forward declare this function from source so it can be acessed in the krnl
void* realloc(void* address, uint_64 newAllocSize); //Forward declare this function from source so it can be acessed in the krnl
void* aligned_alloc(uint_64 alignment, uint_64 size);