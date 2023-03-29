#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "IF.h"
#include "ID.h"
#include "EX.h"
#include "MEM.h"
#include "WB.h"
#include "Resource.h"

I_Memory I_Mem;
D_Memory D_Mem;
Control_Unit Con;
Register_File Reg;
Instruction Inst;
Option Opt;

int Memory[0x400000] = { 0, };
int R[32] = { 0, };
	
void Init();
void Load_Memory(FILE* fp);
void Simul();
void Result(char* filename);

int main(int argc, char* argv[])
{
	FILE* fp = NULL;
	char* filename;
	filename = (char*)malloc(sizeof(char) * 20);

	// identify input argument for bin file .
	if (argc >= 2)
		filename = argv[1];
	else
	{
		printf("No argument\n");
		return 0;
	}

	// identify existence of the file
	fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		printf("No file : '%s'\n", filename);
		return 0;
	}
	else
		printf("Starting MIPS Simulator\n\n");

	Init();
	Load_Memory(fp);
	fclose(fp);

	Simul();
	Result(filename);

	return 0;
}

void Init()
{
	R[31] = 0xFFFFFFFF;
	R[29] = 0x01000000;
	Cycle_Counter = 0;
	PC = 0;
	M_index = 0;
	J_addr = 0;
	B_addr = 0;
	result = 0;
	bcond = 0;
	MUX4_pc = 0;

	memset(&I_Mem, 0, sizeof(I_Memory));
	memset(&D_Mem, 0, sizeof(D_Memory));
	memset(&Con, 0, sizeof(Control_Unit));
	memset(&Reg, 0, sizeof(Register_File));
	memset(&Inst, 0, sizeof(Instruction));
	memset(&Opt, 0, sizeof(Option));
}

void Load_Memory(FILE* fp)
{
	for(int i = 0; feof(fp) == 0; i++)
	{
		fread(&Memory[i], sizeof(int), 1, fp);
		Memory[i] = ntohl(Memory[i]);
	}
}

void Simul()
{
	while (PC != 0xFFFFFFFF)
	{
		Cycle_Counter++;
		Fetch();
		Decode();
		Execute();
		Memory_Access();
		WriteBack();

		printf("Cycle : %d\n[Changed Architectural State]\n", Cycle_Counter);
		printf("->PC : 0x%08x\n", M_index * 4);
		printf("->Memory : 0x%08x\n", Memory[M_index]);
		switch (Inst.opcode)
		{
			case 0x00:
				printf(" ->| opcode : 0x%02x | rs : %d | rt: %d | rd : %d | shamt : 0x%02x | funct : 0x%02x |\n", Inst.opcode, Inst.rs, Inst.rt, Inst.rd, Inst.shamt, Inst.funct);
				break;
			case 0x02:
			case 0x03:
				printf(" ->| opcode : 0x%02x | address : 0x%08x |\n", Inst.opcode, Inst.address);
				break;
			default:
				printf(" ->| opcode : 0x%02x | rs : %d | rt : %d | imm : 0x%04x |\n", Inst.opcode, Inst.rs, Inst.rt, Inst.imm);
				break;
		}
		printf("->Register\n");
		printf(" ->| R[rs] : 0x%08x | R[rt] : 0x%08x | R[rd] : 0x%08x | R[2] : 0x%08x |\n\n", R[Inst.rs], R[Inst.rt], R[Inst.rd], R[2]);
	}
}

void Result(char* filename)
{
	printf("[%s Result]\n", filename);
	printf("->Instruction Num : %d\n", Cycle_Counter);
	printf(" ->| R_type : %d | I_type : %d | J_type : %d |\n", Opt.R_num, Opt.I_num, Opt.J_num);
	printf("->Memory Access Num : %d\n", Opt.Memory_Access_Num);
	printf("->Branch Num : %d\n", Opt.Branch_Taken_Num);
	printf("->V0 : %d\n", R[2]);
}

