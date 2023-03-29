#pragma once
#ifndef ID_INCLUDED
#define ID_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Resource.h"

I_Memory I_Mem;
D_Memory D_Mem;
Control_Unit Con;
Register_File Reg;
Instruction Inst;
Option Opt;

void Decode();
void Control_Func();
void Sign_Extend();
void Jump_Addr();
void Branch_Addr();
void Operand_Fetch();

#endif