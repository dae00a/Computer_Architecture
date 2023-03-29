#include "IF.h"

void Fetch()
{
	M_index = PC / 4;
	I_Mem.I_Address = M_index;
	I_Mem.instruction = Memory[I_Mem.I_Address];
	PC += 4;
}
