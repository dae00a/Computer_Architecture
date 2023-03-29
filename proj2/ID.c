#include "ID.h"

void Decode()
{
	Inst.opcode = ((I_Mem.instruction & 0xFC000000) >> 26) &0x3F;
	Inst.rs = (I_Mem.instruction & 0x03E00000) >> 21;
	Inst.rt = (I_Mem.instruction & 0x001F0000) >> 16;
	Inst.rd = (I_Mem.instruction & 0x0000F800) >> 11;
	Inst.shamt = (I_Mem.instruction & 0x000007C0) >> 6;
	Inst.funct = (I_Mem.instruction & 0x0000003F);
	Inst.imm = (I_Mem.instruction & 0x0000FFFF);
	Inst.address = (I_Mem.instruction & 0x03FFFFFF);

	if (Inst.opcode == 0x00)
		Opt.R_num++;
	else if (Inst.opcode == 0x02 || Inst.opcode == 0x03)
		Opt.J_num++;
	else
		Opt.I_num++;

	Control_Func();
	Sign_Extend();
	Jump_Addr();
	Branch_Addr();
	Operand_Fetch();
}

void Control_Func()
{
	// RegDst
	if ((Inst.opcode == 0x00 && Inst.funct == 0x09) || Inst.opcode == 0x03)
		Con.RegDst = 2;
	else if(Inst.opcode == 0x00)
		Con.RegDst = 1;
	else 
		Con.RegDst = 0;
	// Jump
	if (Inst.opcode == 0x00 && (Inst.funct == 0x08 || Inst.funct == 0x09))
		Con.Jump = 2;
	else if (Inst.opcode == 0x02 || Inst.opcode == 0x03)
		Con.Jump = 1;
	else 
		Con.Jump = 0;
	// Branch
	if (Inst.opcode == 0x04 || Inst.opcode == 0x05)
		Con.Branch = 1;
	else
		Con.Branch = 0;
	// MemRead
	if (Inst.opcode == 0x23)
		Con.MemRead = 1;
	else
		Con.MemRead = 0;
	// MemtoReg
	if (Inst.opcode == 0x00 && Inst.funct == 0x09)
		Con.MemtoReg = 3;
	else if (Inst.opcode == 0x03)
		Con.MemtoReg = 2;
	else if (Inst.opcode == 0x23)
		Con.MemtoReg = 1;
	else
		Con.MemtoReg = 0;
	// ALUOp
	if (Inst.opcode == 0x00)
		Con.ALUOp = 1;
	else
		Con.ALUOp = 0;
	// MemWrite
	if (Inst.opcode == 0x2B)
		Con.MemWrite = 1;
	else 
		Con.MemWrite = 0;
	// ALUSrc
	if (Inst.opcode == 0x00 || Inst.opcode == 0x04 || Inst.opcode == 0x05)
		Con.ALUSrc = 0;
	else 
		Con.ALUSrc = 1;
	// RegWrite
	if ((Inst.opcode == 0x00 && Inst.funct == 0x08) || Inst.opcode == 0x02 || Inst.opcode == 0x2B || Inst.opcode == 0x04 || Inst.opcode == 0x05)
		Con.RegWrite = 0;
	else
		Con.RegWrite = 1;
}

void Sign_Extend()
{
	Inst.Ext_imm = (Inst.imm << 16) >> 16;
	Inst.Zero_Ext_imm = 0x00000000 | Inst.imm;
}

void Jump_Addr()
{
	J_addr = ((PC & 0xf0000000) | (Inst.address << 2));
}

void Branch_Addr()
{
	B_addr = ((Inst.Ext_imm << 16) >> 14);
}

void Operand_Fetch()
{
	Reg.Read_Reg1 = Inst.rs;
	Reg.Read_Reg2 = Inst.rt;
	// MUX2
	if (Con.RegDst == 2)
		Reg.Write_Reg = 31;
	else if (Con.RegDst == 1)
		Reg.Write_Reg = Inst.rd;
	else if (Con.RegDst == 0)
		Reg.Write_Reg = Inst.rt;
	
	Reg.Read_Data1 = R[Reg.Read_Reg1];
	Reg.Read_Data2 = R[Reg.Read_Reg2];
}
