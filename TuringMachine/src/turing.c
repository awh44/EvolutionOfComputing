#include <stdlib.h>

#include "turing.h"

#include "status.h"
#include "string_t.h"

status_t turing_init(turing_t **turing, state_t num, state_t start, state_t accept, state_t reject)
{
	status_t error = SUCCESS;
	if ((*turing = malloc(sizeof **turing)) == NULL)
	{
		error = OUT_OF_MEM;
		goto error0;
	}

	(*turing)->tape = string_initialize();
	if ((*turing)->tape == NULL)
	{
		error = OUT_OF_MEM;
		goto error1;
	}
	(*turing)->tape_pos = 0;


	if ((error = transfunc_init(&(*turing)->func, num)))
	{
		error = OUT_OF_MEM;
		goto error2;
	}

	(*turing)->num_states = num;
	(*turing)->state = start;
	(*turing)->accept = accept;
	(*turing)->reject = reject;

	goto success;

error2:
	string_uninitialize((*turing)->tape);
error1:
	free(*turing);
error0:

success:
	return error;
}

void turing_uninit(turing_t *turing)
{
	transfunc_uninit(turing->func);
	string_uninitialize(turing->tape);
	free(turing);
}

void turing_set_tape(turing_t *turing, char *tape, size_t length)
{
	string_assign_from_char_array_with_size(turing->tape, tape, length);
}

status_t turing_run(turing_t *turing)
{
	status_t error = SUCCESS;

	while (!turing_has_accepted(turing) && !turing_has_rejected(turing))
	{
		//If the head position is past the end of the tape, push a blank symbol
		//onto it
		if (turing->tape_pos == string_length(turing->tape))
		{
			string_push_back(turing->tape, ' ');
		}

		char reading = string_get(turing->tape, turing->tape_pos);
		if ((error = turing_make_transition(turing, turing->state, reading)))
		{
			goto exit0;
		}
	}

exit0:
	return error;
}

status_t turing_make_transition(turing_t *turing, state_t state, char reading)
{
	status_t error = SUCCESS;

	trans_t *trans = transfunc_get_transition(turing->func, state, reading);
	if (trans == NULL)
	{
		error = INVALID_TRANS;
		goto exit0;
	}

	//Update the TM
	turing->state = trans->state;

	string_set(turing->tape, turing->tape_pos, trans->write);

	//If at the left end, the TM can't continue going left
	if ((trans->dir == LEFT) && (turing->tape_pos > 0))
	{
		turing->tape_pos--;
	}
	else if (trans->dir == RIGHT)
	{
		turing->tape_pos++;
	}

exit0:
	return error;
}

unsigned short turing_has_accepted(turing_t *turing)
{
	return turing->state == turing->accept;
}

unsigned short turing_has_rejected(turing_t *turing)
{
	return turing->state == turing->reject;
}

void turing_dump_state(turing_t *turing, FILE *stream)
{
	fprintf
	(
		stream,
		"State: %zu\nTape: %s\nHead position: %zu\n",
		turing->state, string_c_str(turing->tape), turing->tape_pos
	);
}

void turing_print_results(turing_t *turing, FILE *stream)
{
	fprintf
	(
		stream,
		"%s with tape %s\n",
		turing_has_accepted(turing) ? "Accepted" : "Rejected",
		string_c_str(turing->tape)
	);
}
