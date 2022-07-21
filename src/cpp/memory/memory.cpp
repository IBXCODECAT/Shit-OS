#include "./memory.h"

/// <summary>
/// Set data in memory
/// <summary>
/// <param name=start>the memory adress we want to start setting data at</param>
/// <param name=value>the value we want to set in all mmeory locations availible</param>
/// <param name=num>The size of the address block we are going to be setting values in</param>
void MemSet(void* start, uint_64 value, uint_64 num)
{
    //If the blocksize is less than 8...
    if(num <= 8)
    {
        //Get an 8 bit pointer to a 64 bit value
        uint_8* valPtr = (uint_8*)&value;

        //Set the memory byte by byte
        for(uint_8* ptr = (uint_8*)start; ptr < (uint_8*)((uint_64)start + num); ptr++)
        {
            *ptr = *valPtr;
            valPtr++;
        }

        return;
    }
    
    uint_64 proceedingBytes = num % 8; //Calculate how many bytes we have to set
    uint_64 newNum = num - proceedingBytes; //Calculate how many 64 bit set operations we can do

    //Set the memory in large 64 bit chunks
    for(uint_64* ptr = (uint_64*)start; ptr < (uint_64*)((uint_64)start + newNum); ptr++)
    {
        *ptr = value;
    }

    //Get an 8 bit pointer to a 64 bit value
    uint_8* valPtr = (uint_8*)&value;

    //Finish off by setting the memory byte by byte
    for(uint_8* ptr = (uint_8*)((uint_64)start + newNum); ptr < (uint_8*)((uint_64)start + num); ptr++)
    {
        *ptr = *valPtr;
        valPtr++;
    }

    return;


}