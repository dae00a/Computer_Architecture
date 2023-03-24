/*
	This code is a simple calculator corresponding to the input string. 
	It gets input string from each line one by one in given text file, and prints out display along with the architectural state changes. 
	That is, the calculator following Von Neumann model is implemented with memory, instruction registers, and instruction pointer.
	Written by : 32173387 Lee JeongHun
	Date: 2021.03.19
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// support registers are defined as int array
int R[10];
// flag is defined as int for identifying interrupt situation
int flag;

typedef struct INSTRUCTION {
	char op;
	// identifying instruction type with the number of operands
	int op_type;
	int opr1;
	int opr2;
	// 0: use reg, 1: use num
	int mod1;
	int mod2;
} INSTRUCTION;

void find_address(int* IP, FILE* fp);
void fetch(char* IR, FILE* fp);
void decode(char* IR, INSTRUCTION* inst);
void decode_operand(char* str, int* opr, int* mod);
void load_from_reg(INSTRUCTION* inst);
void execute(INSTRUCTION* inst, int* IP, FILE* fp);

int main(int argc, char* argv[]) {
    FILE *fp = NULL;
    char *filename = NULL;
	INSTRUCTION inst;
    // IR(Instruction Register)
    char IR[200];
    // IP(Instruction Pointer)
    int IP[20];

	if(argc < 2) {
		printf("E: File name has to typed as an argument!\n");
		exit(1);
	}
    else
		filename = argv[1];
        
    fp = fopen(filename, "r");
    if(fp == NULL) {
		printf("E: no file found\n");
		exit(1);
    }
    
	find_address(IP, fp);
	
    printf("opening %s: \n", filename);
	do {
		fetch(IR, fp);
		if (flag)
			break;
		decode(IR, &inst);
		if (flag)
			break;
		execute(&inst, IP, fp);
		if (flag)
			break;
		printf("\n");
	} while (feof(fp) == 0);

    fclose(fp);

    return 0;
}

//	function that stores each address of instructions
void find_address(int* IP, FILE* fp) {
	char instruction[16];

	IP[1] = ftell(fp);
	for (int j = 2; fgets(instruction, 255, fp) != NULL; j++)
		IP[j] = ftell(fp);

	rewind(fp);
}

//	function that fetches a instruction from memory
void fetch(char* IR, FILE* fp) {
	char* inst;
	int len;

	inst = fgets(IR, 255, fp);
	len = strlen(IR);
	
	if (IR[len - 1] == '\n')
		IR[len - 1] = 0;
	if (inst == NULL)
		flag = 1;
	else
		printf("Fet: %s\n", IR);
}

//	function that decodes a instruction
void decode(char* IR, INSTRUCTION *inst) {
	memset(inst, 0, sizeof(*inst));

	char* token = strtok(IR, " ");	
	if (token == NULL)
		flag = 1;
	else {
		inst->op = token[0];
		switch (inst->op) {
		case '+':
		case '-':
		case '*':
		case '/':
		case 'M':
		case 'C':
			inst->op_type = 2;
			break;
		case 'J':
		case 'B':
			inst->op_type = 1;
			break;
		case 'H':
			inst->op_type = 0;
			break;
		default:
			flag = 1;
			break;
		}
		printf("Dec: %s.", token);
		
		if (inst->op_type == 1) {
			token = strtok(NULL, " ");
			decode_operand(token, &inst->opr1, &inst->mod1);
			printf("0x%X.0x%X.0x0.0x0\n", inst->opr1, inst->mod1);
		}
		else if (inst->op_type == 2) {
			for (int j = 1; token != NULL; j++) {
				token = strtok(NULL, " ");
				if (j == 1)
					decode_operand(token, &inst->opr1, &inst->mod1);
				else if (j == 2)
					decode_operand(token, &inst->opr2, &inst->mod2);
			}
			printf("0x%X.0x%X.0x%X.0x%X\n", inst->opr1, inst->mod1, inst->opr2, inst->mod2);
		}
		else
			printf("0x0.0x0.0x0.0x0\n");
	}
	IR[0] = '\0';
}

//	function that decodes the instruction's operands
void decode_operand(char* str, int* opr, int* mod)
{
	int i = 0;

	if (strlen(str) == 0) {
		printf("E: There are no operands");
		exit(1);
	}
	else if (strstr(str, "R") != NULL) {
		*mod = 0;
		i = strtol(str + 1, NULL, 10);
		*opr = i;
	}
	else if (strstr(str, "0x") != NULL) {
		*mod = 1;
		i = strtol(str + 2, NULL, 16);
		*opr = i;
	}
}

//	function that gets operands
void load_from_reg(INSTRUCTION* inst) {
	if (inst->mod1 == 0)
		inst->opr1 = R[inst->opr1];
	if (inst->mod2 == 0)
		inst->opr2 = R[inst->opr2];
}

//	function that executes a instruction
void execute(INSTRUCTION* inst, int* IP, FILE* fp)
{
	printf("Exe: ");
	switch (inst->op) {
		case '+':
			load_from_reg(inst);
			R[0] = inst->opr1 + inst->opr2;
			printf("R0: 0x%X = 0x%X + 0x%X\n", R[0], inst->opr1, inst->opr2);
			break;
		case '-':
			load_from_reg(inst);
			R[0] = inst->opr1 - inst->opr2;
			printf("R0: 0x%X = 0x%X - 0x%X\n", R[0], inst->opr1, inst->opr2);
			break;
		case '*':
			load_from_reg(inst);
			R[0] = inst->opr1 * inst->opr2;
			printf("R0: 0x%X = 0x%X * 0x%X\n", R[0], inst->opr1, inst->opr2);
			break;
		case '/':
			load_from_reg(inst);
			R[0] = inst->opr1 / inst->opr2;
			printf("R0: 0x%X = 0x%X / 0x%X\n", R[0], inst->opr1, inst->opr2);
			break;
		case 'M':
			if (inst->mod2 == 0)
				inst->opr2 = R[inst->opr2];
			R[inst->opr1] = inst->opr2;
			printf("R%d: 0x%X\n", inst->opr1, inst->opr2);
			break;
		case 'C':
			if (inst->mod1 == 0 && inst->mod2 == 0) {
				if (R[inst->opr1] >= R[inst->opr2]) {
					R[0] = 0;
					printf("R%d >= R%d -> R0: 0\n", inst->opr1, inst->opr2);
				}
				else {
					R[0] = 1;
					printf("R%d < R%d -> R0: 1\n", inst->opr1, inst->opr2);
				}
			}
			else if (inst->mod1 == 0 && inst->mod2 == 1) {
				if (R[inst->opr1] >= inst->opr2) {
					R[0] = 0;
					printf("R%d >= 0x%X -> R0: 0\n", inst->opr1, inst->opr2);
				}
				else {
					R[0] = 1;
					printf("R%d < 0x%X -> R0: 1\n", inst->opr1, inst->opr2);
				}
			}
			else if (inst->mod1 == 1 && inst->mod2 == 0) {
				if (inst->opr1 >= R[inst->opr2]) {
					R[0] = 0;
					printf("0x%X >= R%d -> R0: 0\n", inst->opr1, inst->opr2);
				}
				else {
					R[0] = 1;
					printf("0x%X < R%d -> R0: 1\n", inst->opr1, inst->opr2);
				}
			}
			else if (inst->mod1 == 1 && inst->mod2 == 1) {
				if (inst->opr1 >= inst->opr2) {
					R[0] = 0;
					printf("0x%X >= 0x%X -> R0: 0\n", inst->opr1, inst->opr2);
				}
				else {
					R[0] = 1;
					printf("0x%X < 0x%X -> R0: 1\n", inst->opr1, inst->opr2);
				}
			}
			break;
		case 'J':
			if (inst->mod1 == 0)
				inst->opr1 = R[inst->opr1];
			fseek(fp, IP[inst->opr1], SEEK_SET);
			printf("IP = 0x%X\n", inst->opr1);
			break;
		case 'B':
			if (inst->mod1 == 0)
				inst->opr1 = R[inst->opr1];
			if (R[0] == 1) {
				fseek(fp, IP[inst->opr1], SEEK_SET);
				printf("R0: 1, IP = 0x%X\n", inst->opr1);
			}
			else
				printf("R0: 0, Not Jump\n");

			break;
		case 'H':
			printf("Halt\n");
			flag = 1;
			break;
		default:
			flag = 1;
			break;
	}
}
