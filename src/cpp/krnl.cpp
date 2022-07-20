#include "panic64.h" //Include the panic64.h header
#include "print.h" //Include the print.h header
#include "idt.h" //Include the IDT.h header

/// <summary>
/// Set's the first to characters in video memory to 'P' with a redish background color
/// </summary>
extern "C" void _krnlTest()
{
	int* ptr = (int*)0xb8000;
	*ptr = 0x50505050;
	return;
}

/// <summary>
/// The start of our krnl
/// </summary>
extern "C" void _start()
{
	initIDT();

	SetCursorPosition(CursorPositionFromCoords(0, 0)); //Set the cursor to the very first part of our video memory
	
	//PANIC(); //test
	return;
}
