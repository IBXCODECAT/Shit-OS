#include "./headers/panic64.h" //Include the panic64.h header
#include "./headers/print.h" //Include the print.h header
#include "./headers/idt.h" //Include the IDT.h header
#include "./headers/keyboard.h" //Include the keyboard.h header

#include "./memory/map.h" //Include our memoryMap header
#include "./memory/heap.h" //Include our memoryHeap header

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

/// <summary>
/// The start of our krnl
/// </summary>
extern "C" void _start()
{
	initIDT(); //Initialize the Interupt Descriptor Table

	SetCursorPosition(CursorPositionFromCoords(0, 0)); //Set the cursor to the very first part of our video memory
	
	MainKeyboardHandler = KeyboardHandler;

	MemoryMapEntry** UseableMemoryMaps = GetUseableMemoryRegions();

	InitHeap(0x100000, 0x100000);

	uint_64* testAdress = (uint_64*)aligned_alloc(0x4000, 0x08);
	PrintString(HexToString((uint_64)testAdress));
	PrintString("\r\n");
	//free(testAdress);

	uint_64* testAdress0 = (uint_64*)malloc(0x4000);
	PrintString(HexToString((uint_64)testAdress0));
	PrintString("\r\n");

	return;
}