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

/// <summary>
/// Copy memroy to another memory adress (MemCopy)
/// </summary>
/// <param name=destination>Where we are copying data to</param>
/// <param name=source>Where the data we are copying is</param>
/// <param name=numBytes>The number of bytes to copy from each address</param>
void MemCopy(void* destination, void* source, uint_64 numBytes)
{
    //If the blocksize is less than 8...
    if(numBytes <= 8)
    {
        //Get an 8 bit pointer to a 64 bit value
        uint_8* valPtr = (uint_8*)source;

        //Set the destination the memory byte by byte
        for(uint_8* ptr = (uint_8*)destination; ptr < (uint_8*)((uint_64)destination + numBytes); ptr++)
        {
            *ptr = *valPtr;
            valPtr++;
        }

        return;
    }

    uint_64 proceedingBytes = numBytes % 8; //Calculate how many bytes we have to set
    uint_64 newNum = numBytes - proceedingBytes; //Calculate how many 64 bit set operations we can do
    uint_64* sourcePtr = (uint_64*)source;

    //Set the memory in large 64 bit chunks
    for(uint_64* destPtr64 = (uint_64*)destination; destPtr64 < (uint_64*)((uint_64)destination + newNum); destPtr64++)
    {
        *destPtr64 = *sourcePtr;
        sourcePtr++;
    }

    //Get an 8 bit pointer to a 64 bit value
    uint_8* sourcePtr8 = (uint_8*)((uint_64)source + newNum);

    //Finish off by setting the memory byte by byte
    for(uint_8* destPtr8 = (uint_8*)((uint_64)destination + newNum); destPtr8 < (uint_8*)((uint_64)destination + numBytes); destPtr8++)
    {
        *destPtr8 = *sourcePtr8;
        sourcePtr8++;
    }
}