#pragma once
#ifndef MEM_INCLUDED
#define MEM_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Resource.h"

D_Memory D_Mem;
Control_Unit Con;
Register_File Reg;
Option Opt;

void Memory_Access();

#endif
