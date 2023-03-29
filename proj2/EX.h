#pragma once
#ifndef EX_INCLUDED
#define EX_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "Resource.h"

Control_Unit Con;
Register_File Reg;
Instruction Inst;
Option Opt;
int ALU_input2;

void Evaluate();
void Execute();

#endif