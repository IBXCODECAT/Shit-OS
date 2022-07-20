#pragma once //the compiler will only include this file once
#include "typedefs.h" //Include typedef.h header (contains custom data types)

bool leftShiftPressed = false;
bool rightShiftPressed = false;
uint_8 lastScancode;

void StandardKeyboardHandler(uint_8 scancode, uint_8 chr)
{
	if(chr != 0)
	{
		if(leftShiftPressed || rightShiftPressed)
		{
			PrintChar(chr - 32); //subtracting 32 shifts the ASCII value to an upercase value
		}	
		else
		{
			PrintChar(chr);
		}
	}
	else
	{
		switch(scancode)
		{
			case 0x8e: //Backspace
				SetCursorPosition(CursorPosition - 1);
				PrintChar(' ');
				SetCursorPosition(CursorPosition - 1);
				break;
			case 0x2a: //Left shift pressed
				leftShiftPressed = true;
				break;
			case 0xaa: //Left shift released
				leftShiftPressed = false;
				break;
			case 0x36:
				rightShiftPressed = true;
				break;
			case 0xb6:
				rightShiftPressed = false;
				break;
			case 0x9c: //Enter
				PrintString("\n\r");
				break;
		}
	}
}

//The scancode of 0xE0 represents an arrow key...
void KeyboardHandler0xE0(uint_8 scancode)
{
	switch (scancode)
	{
		case 0x50: //Down arrow
			SetCursorPosition(CursorPosition + VGA_WIDTH);
			break;
		case 0x48: //Up arrow
			SetCursorPosition(CursorPosition - VGA_WIDTH);
			break;
		default:
			break;
	}
}

void KeyboardHandler(uint_8 scancode, uint_8 chr)
{
	switch (lastScancode)
	{
		case 0xe0:
			KeyboardHandler0xE0(scancode);
			break;
		default:
			StandardKeyboardHandler(scancode, chr);
			break;
	}

	lastScancode = scancode; //Store the last scancode (some keys produce multiple scancodes)
}