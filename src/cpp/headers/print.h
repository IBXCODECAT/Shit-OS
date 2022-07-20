#pragma once //the compiler will only include this file once

#include "typedefs.h" //Include typedef.h header (contains custom data types)
#include "IO.h" //Include the IO.h header (contains IO bus functions)
#include "txtcolors.h" //Include the txtcolor.h header (contains color formatting definitions)

#define VGA_MEMORY (unsigned char*)0xb8000 //Unsigned char pointer to video memory adress
#define VGA_WIDTH 80
#define VGA_MEMORY_SIZE 4000

uint_16 CursorPosition;

/// <summary>
/// Sets our cursor position at an index that ranges from zero to the end of our video memory
/// </summary>
/// <param name="position">A uint_16 representing the position to place the cursor</param>
void SetCursorPosition(uint_16 position)
{
	outb(0x304, 0x0f);
	outb(0x3d5, (uint_8)(position & 0xff));
	outb(0x3d4, 0x0e);
	outb(0x3d5, (uint_8)((position >> 8) & 0xff));

	CursorPosition = position;
}

uint_16 CursorPositionFromCoords(uint_8 x, uint_8 y)
{
	return y * VGA_WIDTH + x;
}

void ClearScreen(uint_64 clearColor = BACKGROUND_BLACK | FOREGROUND_WHITE, bool resetCursor = false)
{
	uint_64 value64 = 0;
	value64 += clearColor << 8;
	value64 += clearColor << 24;
	value64 += clearColor << 40;
	value64 += clearColor << 56;

	//Note i++ increments by 8 due to the 64 bit pointer
	for (uint_64* i = (uint_64*)VGA_MEMORY; i < (uint_64*)(VGA_MEMORY + VGA_MEMORY_SIZE); i++)
	{
		*i = value64; //Dereference i to value
	}

	if (resetCursor)
	{
		SetCursorPosition(0);
	}
}

void PrintString(const char* str, uint_8 color = BACKGROUND_BLUE | FOREGROUND_WHITE)
{
	uint_8* charPtr = (uint_8*)str; //Creating a new value so we don't alter the adress of the parameter
	uint_16 index = CursorPosition;

	while (*charPtr != 0) //While our string has not reached null terminiation
	{
		switch (*charPtr)
		{
			case 10: //Is the character a newline character?
				index += VGA_WIDTH; //Increment the video index by the width of the screen (down one row)
				break;
			case 13: //Is the character a return formatting character?
				index -= index % VGA_WIDTH; //Move the video memory index back to the begining of this row
				break;
			default:
				*(VGA_MEMORY + index * 2) = *charPtr; //Dereference video memory at index (refrence every second value to skip formating values)
				*(VGA_MEMORY + index * 2 + 1) = color; //Dereference video memory at index (refrence every second value to skip formating values)
				index++; //Move the video memory index for the next character
				break;
		}

		charPtr++;
	}

	SetCursorPosition(index); //Place our cursor at the end of the string we printed
}

void PrintChar(char chr, uint_8 color = BACKGROUND_BLUE | FOREGROUND_WHITE)
{
	*(VGA_MEMORY + CursorPosition * 2) = chr;
	*(VGA_MEMORY + CursorPosition * 2 + 1) = color;
	SetCursorPosition(CursorPosition + 1);

}

char hexToStringOut[128];

/// <summary>
/// Converts a hex value to a string OR converts a string to another string representing its location in memory as hex
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="value"></param>
/// <returns></returns>
template<typename T> const char* HexToString(T value)
{
	T* valPtr = &value;
	uint_8* ptr; //Oh pointers! Aren't they so much fun!
	uint_8 tmp; //Temp variable for extracting letters and numbers from hex value
	uint_8 size = (sizeof(T)) * 2 - 1; //The size of T multiplied by two (skip formatting bytes) - 1
	uint_8 i; //The index for our loop

	for (i = 0; i < size; i++)
	{
		ptr = ((uint_8*)valPtr + i);

		//Left side of hexidecimal value
		tmp = ((*ptr & 0xF0) >> 4); //Extracts the left part of the hexidecimal and gives us its value
		hexToStringOut[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : 48); //Convert hex to ASCII value

		//Right side of hexidecimal value
		tmp = ((*ptr & 0x0F) >> 4); //Extracts the right part of the hexidecimal and gives us its value
		hexToStringOut[size - (i * 2 + 0)] = tmp + (tmp > 9 ? 55 : 48); //Convert hex to ASCII value
	}

	hexToStringOut[size + 1] = 0; //Keeps our string null-terminated so we can seperate our strings

	return hexToStringOut;
}