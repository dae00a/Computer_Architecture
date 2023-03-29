#include "EX.h"

void Evaluate()
{
	// MUX3
	if (Con.ALUSrc)
		ALU_input2 = Inst.Ext_imm;
	else
		ALU_input2 = Reg.Read_Data2;
}
void Execute()
{
	Evaluate();

	if (Con.ALUOp)
	{
		switch (Inst.funct)
		{
			case 0x20:
			case 0x21:
				result = Reg.Read_Data1 + ALU_input2;
				break;
			case 0x24:
				result = Reg.Read_Data1 & ALU_input2;
				break;
			case 0x27:
				result = ~(Reg.Read_Data1 | ALU_input2);
				break;
			case 0x25:
				result = (Reg.Read_Data1 | ALU_input2);
				break;
			case 0x2A:
			case 0x2B:
				result = (Reg.Read_Data1 < ALU_input2) ? 1 : 0;
				break;
			case 0x00:
				result = (Reg.Read_Data2 << Inst.shamt);
				break;
			case 0x02:
				result = (Reg.Read_Data2 >> Inst.shamt);
				break;
			case 0x22:
			case 0x23:
				result = Reg.Read_Data1 - ALU_input2;
				break;
			default:
				break;
		}
	}
	else
	{
		switch (Inst.opcode)
		{
			case 0x8:
			case 0x9:
				result = Reg.Read_Data1 + ALU_input2;
				break;
			case 0xc:
				result = Reg.Read_Data1 & Inst.Zero_Ext_imm;
				break;
			case 0x4:
				if (Reg.Read_Data1 == ALU_input2)
				{
					bcond = 1;
					Opt.Branch_Taken_Num++;
				}
				else
					bcond = 0;
				break;
			case 0x5:
				if (Reg.Read_Data1 != ALU_input2)
				{
					bcond = 1;
					Opt.Branch_Taken_Num++;
				}
				else
					bcond = 0;
				break;
			case 0x30:
			case 0x23:
				result = Reg.Read_Data1 + ALU_input2;
				break;
			case 0xF:
				result = (ALU_input2 << 16);
				break;
			case 0x0D:
				result = Reg.Read_Data1 | Inst.Zero_Ext_imm;
				break;
			case 0x0A:
			case 0x0B:
				result = (Reg.Read_Data1 < ALU_input2) ? 1 : 0;
				break;
			case 0x2B:
				result = Reg.Read_Data1 + ALU_input2;
				break;
		}
	}
	// PC update
	MUX4_pc = PC;
	// MUX1
	if (Con.Jump == 2)
		PC = Reg.Read_Data1;
	else if (Con.Jump == 1)
	{	
		PC = J_addr;
	}
	else if (Con.Jump == 0)
	{
		// MUX0
		if (bcond && Con.Branch)
			PC += B_addr;
	}
}
