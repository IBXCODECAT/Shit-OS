#include "./headers/panic64.h" //Include the panic64.h header
#include "./headers/print.h" //Include the print.h header
#include "./headers/idt.h" //Include the IDT.h header


extern const char binTest[]; //Creates a char array from the binaries we loaded in bin.asm

/// <summary>
/// Set's the first to characters in video memory to 'P' with a redish background color
/// </summary>
extern "C" void _krnlTest()
{
	int* ptr = (int*)0xb8000;
	*ptr = 0x50505050;
	return;
}

void KeyboardHandler(uint_8 scancode, uint_8 chr)
{
	if(chr != 0)
	{
		PrintChar(chr);	
	}
}

/// <summary>
/// The start of our krnl
/// </summary>
extern "C" void _start()
{
	initIDT(); //Initialize the Interupt Descriptor Table

	SetCursorPosition(CursorPositionFromCoords(0, 0)); //Set the cursor to the very first part of our video memory
	
	MainKeyboardHandler = KeyboardHandler;

	//PANIC(); //test

	PrintString(binTest); //print out our binaries to the screen

	return;
}