#pragma once //the compiler will only include this file once

#include "./../headers/typedefs.h"

void MemSet(void* start, uint_64 value, uint_64 num);

void MemCopy(void* destination, void* source, uint_64 numBytes);