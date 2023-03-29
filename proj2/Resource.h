#pragma once
#ifndef RESOURCE_INCLUDED
#define RESOURCE_INCLUDED

#include <stdio.h>

/*
* -R Type-
* add    0/0x20
* addu   0/0x21
* and    0/0x24
* jr     0/0x08
* jalr   0/0x09
* nor    0/0x27
* or     0/0x25
* slt    0/0x2A
* sltu   0/0x2B
* sll    0/0x00
* srl    0/0x02
* sub    0/0x22
* subu   0/0x23
* -J Type-
* j      0x02
* jal    0x03
* -I Type-
* addi   0x08
* addiu  0x09
* andi   0x0C
* beq    0x04
* bne    0x05
* ll     0x30
* lui    0x0F
* lw     0x23
* ori    0x0D
* slti   0x0A
* sltiu  0x0B
* sw     0x2B
*/

// 16MB = 0x400000 * 4Byte
int Memory[0x400000], R[32];
unsigned int Cycle_Counter, PC;
int M_index, J_addr, B_addr;
int result; // ALU result
int bcond; // ALU branch condition
int MUX4_pc; // variable for input of MUX4 before updating PC

typedef struct 
{
	int I_Address;
	int instruction;
}I_Memory;

typedef struct
{
	int D_Address;
	int D_Write_Data;
	int D_Read_Data;
}D_Memory;

typedef struct 
{
	int RegDst;
	int Jump;
	int Branch;
	int MemRead;
	int MemtoReg;
	int ALUOp;
	int MemWrite;
	int ALUSrc;
	int RegWrite;
}Control_Unit;

typedef struct 
{
	int Read_Reg1;
	int Read_Reg2;
	int Write_Reg;
	int Write_Data;
	int Read_Data1;
	int Read_Data2;
}Register_File;

typedef struct 
{
	unsigned int opcode;
	unsigned int rs;
	unsigned int rt;
	unsigned int rd;
	signed int imm;
	signed int Ext_imm;
	signed int Zero_Ext_imm;
	unsigned int funct;
	unsigned int shamt;
	unsigned int address;
}Instruction;

typedef struct 
{
	int R_num;
	int I_num;
	int J_num;
	int Memory_Access_Num;
	int Branch_Taken_Num;
}Option;

#endif