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

	for(uint_8 i = 0; i < useableMemoryRegionsCount; i++)
	{
		MemoryMapEntry* memMap = UseableMemoryMaps[i];
		PrintMemoryMap(memMap, CursorPosition);
		PrintString("\n\r");
	}

	InitHeap(0x100000, 0x100000); //Initialize our heap with the maximun size we set previously

	void* testMemoryadress = malloc(60);

	PrintString(HexToString((uint_64)testMemoryadress));


	return;
}