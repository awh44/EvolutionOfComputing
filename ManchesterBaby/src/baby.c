#include <stdlib.h>

#include "baby.h"

#include "status.h"

static status_t baby_execute_instruction(baby_t *baby, uint8_t *cont);

status_t baby_init(baby_t **out_baby)
{
	status_t error = SUCCESS;
	baby_t *baby;
	if ((baby = calloc(1, sizeof(*baby))) == NULL)
	{
		error = OUT_OF_MEM;
		goto error0;
	}

	*out_baby = baby;
	goto success;

error0:

success:
	return error;
}

void baby_uninit(baby_t *baby)
{
	free(baby);
}

status_t baby_load_value(baby_t *baby, uint32_t addr, int32_t value)
{
	status_t error = SUCCESS;
	if (addr  > BABY_MEMSIZE)
	{
		error = INVALID_ADDRESS;
		goto exit;
	}

	baby->memory[addr] = value;

exit:
	return error;
}

status_t baby_load_current_instruction(baby_t *baby, uint32_t addr)
{
	status_t error = SUCCESS;
	if (addr > BABY_MEMSIZE)
	{
		error = INVALID_ADDRESS;
		goto exit;
	}

	baby->ci = addr;

exit:
	return error;
}

#define INSTR_OPCODE(instr)  (((instr) >> 13) & 7)
#define INSTR_ADDRESS(instr) ((instr) & 0x1FFF)
typedef enum
{
	JUMP = 0,
	JUMP_RELATIVE = 1,
	LOAD = 2,
	STORE = 3,
	SUB1 = 4,
	SUB2 = 5,
	SKIP_IF_NEG = 6,
	STOP = 7,
} opcode_t;

static status_t baby_execute_instruction(baby_t *baby, uint8_t *cont)
{
	status_t error = SUCCESS;
	if (baby->ci >= BABY_MEMSIZE)
	{
		error = INVALID_ADDRESS;
		goto exit0;
	}

	//Update the informational present instruciton register
	baby->pi = baby->memory[baby->ci];

	//Advance to next instruction
	baby->ci++;

	//Extract the address and the value at that address, in case they'll
	//be needed.
	uint16_t addr = INSTR_ADDRESS(baby->pi);
	if (addr >= BABY_MEMSIZE)
	{
		error = INVALID_ADDRESS;
		goto exit0;
	}
	int32_t value = baby->memory[addr];

	//Default to continuing for all instructions but STOP
	*cont = 1;

	switch (INSTR_OPCODE(baby->pi))
	{
		case JUMP:
			baby->ci = value;
			break;
		case JUMP_RELATIVE:
			baby->ci += value;
			break;
		case LOAD:
			baby->accumulator = -value;
			break;
		case STORE:
			baby->memory[addr] = baby->accumulator;
			break;
		case SUB1:
		case SUB2:
			baby->accumulator -= value;
			break;
		case SKIP_IF_NEG:
			if (baby->accumulator < 0)
			{
				baby->ci++;
			}
			break;
		case STOP:
			*cont = 0;
			break;
	}

exit0:
	return error;
}

status_t baby_execute(baby_t *baby)
{
	status_t error = SUCCESS;
	uint8_t cont;
	while (!(error = baby_execute_instruction(baby, &cont)) && cont);
	return error;
}

status_t baby_execute_num_steps(baby_t *baby, uint32_t num_steps)
{
	status_t error = SUCCESS;
	uint8_t cont;
	for (uint32_t i = 0;
	     (i < num_steps) && !(error = baby_execute_instruction(baby, &cont)) && cont; 
		 i++);
	return error;
}

void baby_display(baby_t *baby, FILE *stream)
{
	fprintf(stream,
"Accumulator: 0x%08x\n\
Current instruction pointer: 0x%08x\n\
Present instruction: 0x%08x\n\
Memory:\n",
	baby->accumulator, baby->ci, baby->pi);

	for (size_t i = 0; i < BABY_MEMSIZE / 4; i++)
	{
		fprintf(stream, "0x%08x", baby->memory[i * 4]);
		for (size_t j = 1; j < 4; j++)
		{
			fprintf(stream, " 0x%08x", baby->memory[i * 4 + j]);
		}
		fprintf(stream, "\n");
	}
}
