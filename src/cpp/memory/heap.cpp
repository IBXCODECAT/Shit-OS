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