#pragma once //the compiler will only include this file once

#include "./../headers/typedefs.h"

void CreateWindow(uint_8 drawPointX, uint_8 drawPointY, uint_8 height, uint_8 width); //Declare here to make acessable to krnl

void DrawBox(uint_8 drawPointX, uint_8 drawPointY, uint_8 height, uint_8 width, uint_64 colorByte); //Declare here to make avail for krnl