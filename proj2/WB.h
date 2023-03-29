#pragma once
#ifndef WB_INCLUDED
#define WB_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Resource.h"

D_Memory D_Mem;
Control_Unit Con;
Register_File Reg;

void WriteBack();

#endif