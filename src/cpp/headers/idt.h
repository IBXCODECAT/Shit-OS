#pragma once //the compiler will only include this file once

#include "typedefs.h" //Include typedef.h header (contains custom data types)
#include "keyboardscanset0.h" //Include keybaord scanset header

//The data structure for our interupt descriptor table
struct IDT64
{
	uint_16 offset_low;
	uint_16 selector;
	uint_8 ist;
	uint_8 types_attr;
	uint_16 offset_mid;
	uint_32 offset_high;
	uint_32 zero;
};

extern IDT64 _idt[256]; //There are 256 different interupts that we can handle (externaly defined w/ linker)
extern uint_64 isr1; //The address of our isr1 function in idt.asm
extern "C" void LoadIDT();

/// <summary>
/// Initialize the IDT (Interupt Descriptor Table)
/// </summary>
void initIDT()
{
	_idt[1].zero = 0;
	_idt[1].ist = 0;
		
	_idt[1].selector = 0x08; //The code segment selector declared in GDT
	_idt[1].types_attr = 0x8E; //Declared as a 32-bit interupt gate

	_idt[1].offset_low = (uint_16)(((uint_64)&isr1 & 0x000000000000FFFF));
	_idt[1].offset_mid = (uint_16)(((uint_64)&isr1 & 0x00000000FFFF0000) >> 16);
	_idt[1].offset_high = (uint_32)(((uint_64)&isr1 & 0xFFFFFFFF00000000) >> 32);

	remapPIC();

	//Program the PIC chip
	outb(0x21, 0xfd);
	outb(0xa1, 0xff);

	LoadIDT();
}


void (*MainKeyboardHandler)(uint_8 scancode, uint_8 chr);

extern "C" void isr1_handler()
{
	uint_8 scancode = inb(0x60);
	uint_8 chr = 0;

	if (scancode < 0x3A)
	{
		chr = KBSet0::ScanCodeLookupTable[scancode];
	}

	//If our main keyboard handler is not a null pointer...
	if(MainKeyboardHandler != 0)
	{
		MainKeyboardHandler(scancode, chr);
	}

	//Signal to the PIC chip that we have finished our interput
	outb(0x20, 0x20);
	outb(0xa0, 0x20);
}