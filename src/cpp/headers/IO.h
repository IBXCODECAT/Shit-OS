#pragma once //the compiler will only include this file once

#include "typedefs.h" //Include typedef.h header (contains custom data types)

#define PIC1_CMD 0x20 //Master PIC chip cmd
#define PIC1_DATA 0x21 //Master PIC chip data
#define PIC2_CMD 0xA0 //Slave PIC chip cmd
#define PIC2_DATA 0xA1 //Slave PIC chip data

#define ICW1_INIT 0x10 //Initialization Control Word 1
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

/// <summary>
/// Outputs one byte into a memory address (port) we specify
/// </summary>
/// <param name="port"></param>
/// <param name="value"></param>
void outb(unsigned short port, unsigned char value)
{
	//InLine assembly marked as volatile so the compiler doesn't make optimizations that could break our function
	asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

/// <summary>
/// Reads one byte from a memory address (port) we specify
/// </summary>
/// <param name="port"></param>
/// <returns></returns>
unsigned char inb(unsigned short port)
{
	unsigned char returnVal;

	//InLine assembly marked as volatile so the compiler doesn't make optimizations that could break our function
	asm volatile ("inb %1, %0" : "=a"(returnVal) : "Nd"(port));
	return returnVal;
}

/// <summary>
/// Remap the PIC chip
/// </summary>
void remapPIC()
{
	uint_8 a1, a2; //Store the mask value of the PIC chip
	a1 = inb(PIC1_DATA);
	a2 = inb(PIC2_DATA);

	outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4); //Tell the master PIC chip to start the init sequence
	outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4); //Tell the slave PIC chip to start the init sequence

	outb(PIC1_DATA, 0);
	outb(PIC2_DATA, 8);
	outb(PIC1_DATA, 4);
	outb(PIC2_DATA, 2);
	outb(PIC1_DATA, ICW4_8086);
	outb(PIC2_DATA, ICW4_8086);

	//Restore the PIC masks
	outb(PIC1_DATA, a1);
	outb(PIC2_DATA, a2);
}