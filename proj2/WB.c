#include "WB.h"

void WriteBack()
{	
	if(Con.MemWrite != 1)
	{
		// MUX4
		if(Con.MemtoReg == 3)
			Reg.Write_Data = MUX4_pc;
		else if (Con.MemtoReg == 2)
			Reg.Write_Data = MUX4_pc + 4;
		else if (Con.MemtoReg == 1)
			Reg.Write_Data = D_Mem.D_Read_Data;
		else if (Con.MemtoReg == 0)
			Reg.Write_Data = result;
		
		if (Con.RegWrite == 1)
			R[Reg.Write_Reg] = Reg.Write_Data;
	}
}
