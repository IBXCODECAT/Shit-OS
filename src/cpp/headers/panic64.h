#pragma once //the compiler will only include this file once

#include "print.h" //Include the print.h header file

void PANIC()
{
	ClearScreen(BACKGROUND_BLUE | FOREGROUND_WHITE, true);
	PrintString("A problem has occured and Shit OS has entered a krnl panic to prevent possible\n\rdamage to your device's hardware or software.\n\n\r");
	PrintString("If this is the first time you have encountered this error screen restart your\n\rdevice. If this screen appears again follow these steps:\n\n\r");
	PrintString("Verify that any new hardware or software is properly installed. If this is a new\rinstalation, ask the developers of Shit OS for any upates you might need. If\n\rproblems persist you can file a bug report on our website, or lookup the\n\rerror code bellow in our GitHub documentation.\n\n\r");
	
	PrintString("***ERR***\n\rPAGE_FAULT_IN_NON_PAGED_AREA\n\n\r", FOREGROUND_LIGHTRED | BACKGROUND_BLUE);
	PrintString("***CAUSE***\n\rThe Memory Management Unit attempted to access a memory page without prepping.\n\n\r", FOREGROUND_LIGHTRED | BACKGROUND_BLUE);
	PrintString("***FIX?***\n\rIf writing a krnl privlage program, verify you are calling `free()` correctly.\n\n\r", FOREGROUND_LIGHTRED | BACKGROUND_BLUE);

	PrintString("Beginning dump of physical memory...\n\r");
	PrintString("Memroy will be dumped to '/root/crash.dump'\n\r");
	PrintString("Contact the developers of Shit OS for further assistance\n\n\r");
	PrintString("Memory Dump: [#################################===============] (69% Complete)", FOREGROUND_YELLOW | BACKGROUND_BLUE);
	SetCursorPosition(0);
	
}