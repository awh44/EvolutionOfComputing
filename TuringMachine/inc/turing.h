#ifndef __TURING_H__
#define __TURING_H__

#include <stdio.h>

#include "string_t.h"
#include "transition.h"
#include "status.h"

typedef struct
{
	state_t num_states;
	state_t accept;
	state_t reject;
	transfunc_t *func;

	state_t state;
	string_t *tape;
	size_t tape_pos;
} turing_t;

status_t turing_init(turing_t **turing, state_t num, state_t start, state_t accept, state_t reject);
void turing_uninit(turing_t *turing);
void turing_set_tape(turing_t *turing, char *tape, size_t length);
status_t turing_run(turing_t *turing);
status_t turing_make_transition(turing_t *turing, state_t state, char reading);
unsigned short turing_has_accepted(turing_t *turing);
unsigned short turing_has_rejected(turing_t *turing);
void turing_dump_state(turing_t *turing, FILE *stream);
void turing_print_results(turing_t *turing, FILE *stream);

#endif // __TURING_H__
