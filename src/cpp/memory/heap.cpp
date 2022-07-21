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
/// combines free segments of memroy into a larger segment
/// <summary>
/// <param name=a>The memory segment header of the first block</param>
/// <param name=b>The memory segment header of the second block</param>
void CombineFreeSegments(MemorySegmentHeader* a, MemorySegmentHeader* b)
{
    if(a == 0 || b == 0) return; //If either memory block inputed is null, we won't do anything

    //If a comes before b in memory...
    if(a < b)
    {
        //Make the memory length of a span the entire length of a plus the header for b and the entire length of b
        a->MemoryLength += b->MemoryLength + sizeof(MemorySegmentHeader);

        //Update the nextSegment references for block a 
        a->NextSegment = b->NextSegment;
        a->NextFreeSegment = b->NextFreeSegment;

        b->NextSegment->PreviousSegment = a;
        b->NextSegment->PreviousFreeSegment = a;
        b->NextFreeSegment->PreviousFreeSegment = a;
    }
    else
    {
        //Make the memory length of b span the entire length of b plus the header for a and the entire length of a
        b->MemoryLength += a->MemoryLength + sizeof(MemorySegmentHeader);

        //Update the nextSegment references for block b
        b->NextSegment = a->NextSegment;
        b->NextFreeSegment = a->NextFreeSegment;

        a->NextSegment->PreviousSegment = b;
        a->NextSegment->PreviousFreeSegment = b;
        a->NextFreeSegment->PreviousFreeSegment = b;
    }
}

/// <summary>
/// memory allocate (malloc)
/// <summary>
/// <param name=allocSize>allocation size (how much memory to allocate)</param>
void* malloc(uint_64 allocSize)
{
    //Here, for performance reasons, we align allocation size by 8 bytes so that 64 bit processers can utilize memroy alignment
    uint_64 remainder = allocSize % 8;
    allocSize -= remainder;
    if(remainder != 0) allocSize += 8;

    MemorySegmentHeader* allocatableMemorySegment = firstFreeMemorySegment;

    while(true)
    {
        //If the memory segment length is greater than or equal to the amount of memory requested...
        if(allocatableMemorySegment->MemoryLength >= allocSize)
        {
            //If the length of our memory segment size is larger than our allocation size we can split this segment into two parts:
            //An allocated segment and a free segment
            if(allocatableMemorySegment->MemoryLength > allocSize + sizeof(MemorySegmentHeader))
            {
                //Calculate the position of a new segment header (placed directly after our old segment header plus the size of requested memory allocation)
                MemorySegmentHeader* newSegmentHeader = (MemorySegmentHeader*)((uint_64)allocatableMemorySegment + sizeof(MemorySegmentHeader) + allocSize);
                
                newSegmentHeader->free = true; //This new segment header is free to allocate

                //Calculate the new memory length of the new header...
                newSegmentHeader->MemoryLength = ((uint_64)allocatableMemorySegment->MemoryLength) - sizeof(MemorySegmentHeader) + allocSize;

                //Our new memory segment's next free memory segment will be the same as our allocated segment's next free memory segment
                newSegmentHeader->NextFreeSegment = allocatableMemorySegment->NextFreeSegment;

                //Our new memory segment's next memory segment will be the same as our allocated segment's next memory segment
                newSegmentHeader->NextSegment = allocatableMemorySegment->NextSegment;

                //Our new memory segment's previous free memory segment will be the same as our allocated segment's previous free memory segment
                newSegmentHeader->PreviousFreeSegment = allocatableMemorySegment->NextFreeSegment;

                //Our new memory segment's previous memory segment will be the same as our allocated segment's previous memory segment
                newSegmentHeader->PreviousSegment = allocatableMemorySegment->PreviousSegment;

                //Update the next free segment and next segment propreties of our allocated memory segment to our new free segment
                allocatableMemorySegment->NextFreeSegment = newSegmentHeader;
                allocatableMemorySegment->NextSegment = newSegmentHeader;

                //The memroy length of the allocatable memory segment should be our allocation size
                allocatableMemorySegment->MemoryLength = allocSize;
            }

            //If our currentMemorySegment is our first free memory segment
            if(allocatableMemorySegment == firstFreeMemorySegment)
            {
                //Set the next free memory segment for this segment to the allocatable memory segment
                firstFreeMemorySegment = allocatableMemorySegment->NextFreeSegment;
            }

            allocatableMemorySegment->free = false; //We have allocated the allocatable memory segment so it is no longer free to use
            
            //If the previous memory segment of the allocatable memory segment is not a null pointer
            if(allocatableMemorySegment->PreviousFreeSegment != 0)
            {
                //Make the previous free memory segment's next free segment skip the allocatable memory segment and point to the next actual free segment
                allocatableMemorySegment->PreviousFreeSegment->NextFreeSegment = allocatableMemorySegment->NextFreeSegment;
            }

            //If the next free segment of the allocatable memory segment is not a null pointer
            if(allocatableMemorySegment->NextFreeSegment != 0)
            {
                //Make the next free memory segment's previous free segment skip the allocatable memory segment and point to the next actual next free segment
                allocatableMemorySegment->NextFreeSegment->PreviousFreeSegment = allocatableMemorySegment->PreviousFreeSegment;
            }

            //If the next free segment of the allocatable memory segment is not a null pointer
            if(allocatableMemorySegment->PreviousSegment != 0)
            {
                //Make the previous memory segment's next free segment skip the allocatable memory segment and point to the next actual free segment
                allocatableMemorySegment->PreviousSegment->NextFreeSegment = allocatableMemorySegment->NextFreeSegment;
            }

            //If the next free segment of the allocatable memory segment is not a null pointer
            if(allocatableMemorySegment->NextSegment != 0)
            {
                //Make the next memory segment's previous free segment skip the allocatable memory segment and point to the previous free segment
                allocatableMemorySegment->NextSegment->PreviousFreeSegment = allocatableMemorySegment->PreviousFreeSegment;
            }

            // Return the memory segment that's directly after the header which is free
            return allocatableMemorySegment + 1;
        }

        //If the next free memory segment is a null pointer...
        if(allocatableMemorySegment->NextFreeSegment == 0)
        {
            //OUT OF MEMORY
            return 0; //No memory remaining (hit the physical RAM limit of the system)
        }

        //If the memory length was not big enough fo the alloc size and there is a free segment we shall iterate to the next free memory segment
        allocatableMemorySegment = allocatableMemorySegment->NextFreeSegment;
    }

    return 0; //We should never get here...
}

/// <summary>
/// free memory (free)
/// <summary>
/// <param name=allocSize>the address of the memory we wish to free</param>
void free(void* address)
{
    //Calculate the correct adress of the memory segment header of the memory segment to free
    MemorySegmentHeader* freeableMemorySegment = (MemorySegmentHeader*)address - 1;
    
    freeableMemorySegment->free = true; //This memory segment shall be labeled as free

    //If the freeable memory segment is before the first free memory segment, set the first free memory segment to the freeableMemorySegment
    if(freeableMemorySegment < firstFreeMemorySegment) firstFreeMemorySegment = freeableMemorySegment;

    //If the next  free memory segment of the freeable memory segment is not a null pointer
    if(freeableMemorySegment->NextFreeSegment != 0)
    {
        //If the next free segment's previous free segment is before this free segment... 
        if(freeableMemorySegment->NextFreeSegment->PreviousFreeSegment < freeableMemorySegment)
        {
            //Set the next free segment of the previous free segment to this free segment
            freeableMemorySegment->NextFreeSegment->PreviousFreeSegment = freeableMemorySegment;
        }
    }

    //If the previous free memory segment of the freeable memory segment is not a null pointer
    if(freeableMemorySegment->PreviousFreeSegment != 0)
    {
        //If the pevious free segment's next free segment is after this free segment... 
        if(freeableMemorySegment->PreviousFreeSegment->NextFreeSegment > freeableMemorySegment)
        {
            //Set the previous free segment of the next free segment to this free segment
            freeableMemorySegment->PreviousFreeSegment->NextFreeSegment = freeableMemorySegment;
        }
    }

    //If the next memory segment of the freeable memory segment is not a null pointer
    if(freeableMemorySegment->NextSegment != 0)
    {
        //Set the next memory segment's previous memory segment to this memory segment
        freeableMemorySegment->NextSegment->PreviousSegment = freeableMemorySegment;

        //If the memory segment after the memory segment we are freeing is also free...
        if(freeableMemorySegment->NextSegment->free)
        {
            //Combine both free memory segments into a larger free memory segment
            CombineFreeSegments(freeableMemorySegment, freeableMemorySegment->NextSegment);
        }
    }

    //If the previous memory segment of the freeable memory segment is not a null pointer
    if(freeableMemorySegment->PreviousSegment != 0)
    {
        //Set the next memory segment's next memory segment to this memory segment
        freeableMemorySegment->PreviousSegment->NextSegment = freeableMemorySegment;
        
        //If the memory segment before the memory segment we are freeing is also free...
        if(freeableMemorySegment->PreviousSegment->free)
        {
            //Combine both free memory segments into a larger free memory segment
            CombineFreeSegments(freeableMemorySegment, freeableMemorySegment->PreviousSegment);
        }
    }
}

/// <summary>
/// clear and allocate memory (calloc)
/// <summary>
/// <param name=allocSize>allocation size (how much memory to allocate)</param>
void* calloc(uint_64 allocSize)
{
    void* mallocVal = malloc(allocSize); //Do a malloc call to allocate the memory we want to clear
    MemSet(mallocVal, 0, allocSize); //Zerofill memroy with memset
    return mallocVal;
}

/// <summary>
/// clear and allocate memory (calloc)
/// <summary>
/// <param name=num>the number of items to clear and allocate memroy for</param>
/// <param name=allocSize>allocation size (how much memory to allocate) for each item in an array</param>
void* calloc(uint_64 num, uint_64 allocSize)
{
    return calloc(num * allocSize);
}

/// <summary>
/// reallocate existing memroy
/// </summary>
/// <param name=address>the address of the memory to reallocate (usually returned from a malloc or realloc call)</param>
/// <param name=newAllocSize>The new segment size the realocation function will reserve</param>
void* realloc(void* address, uint_64 newAllocSize)
{
    //Get the memory segment header of the source memory address
    MemorySegmentHeader* sourceSegmentHeader = (MemorySegmentHeader*)address - 1;

    //Calculate the smaller segment size so we don't overflow the memory address
    uint_64 smallSize = newAllocSize;
    if(sourceSegmentHeader->MemoryLength < newAllocSize)
    {
        smallSize = sourceSegmentHeader->MemoryLength;
    }

    void* allocatedMemory = malloc(newAllocSize); //Allocate memroy at destination
    MemCopy(allocatedMemory, address, smallSize); //Copy everything from the old memory adress to the new memory adress
    free(address); //Free the old address as it is no longer in use after reallocation

    return allocatedMemory;
}