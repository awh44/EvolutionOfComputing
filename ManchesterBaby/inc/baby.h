#ifndef __BABY_H__
#define __BABY_H__

#include <stdint.h>
#include <stdio.h>

#include "status.h"

#define BABY_MEMSIZE 32

typedef struct
{
	int32_t memory[BABY_MEMSIZE];
	int32_t accumulator;
	// Current instruction pointer
	uint32_t ci;
	// Present instruction
	int32_t pi;
} baby_t;

status_t baby_init(baby_t **baby);
void baby_uninit(baby_t *baby);
status_t baby_load_value(baby_t *baby, uint32_t addr, int32_t value);
status_t baby_load_current_instruction(baby_t *baby, uint32_t addr);
status_t baby_execute(baby_t *baby);
status_t baby_execute_num_steps(baby_t *baby, uint32_t num_steps);
void baby_display(baby_t *baby, FILE *stream);

#endif // __BABY_H__
