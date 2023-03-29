#include "MEM.h"

void Memory_Access()
{
	if(Con.MemWrite || Con.MemRead)
	{
		D_Mem.D_Address = result;
		D_Mem.D_Write_Data = Reg.Read_Data2;
		if(Con.MemWrite)
			Memory[D_Mem.D_Address / 4] = D_Mem.D_Write_Data;
		if(Con.MemRead)
			D_Mem.D_Read_Data = Memory[D_Mem.D_Address / 4];
		Opt.Memory_Access_Num++;
	}		
}
	