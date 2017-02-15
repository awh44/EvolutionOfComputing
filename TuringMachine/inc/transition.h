#ifndef __TRANSITION_H__
#define __TRANSITION_H__

#include <stddef.h>

#include "status.h"

typedef enum
{
	LEFT,
	RIGHT,
} direction_t;

typedef size_t state_t;

typedef struct trans_t
{
	direction_t dir;
	state_t state;
	char write;
} trans_t;

typedef struct out_t out_t;
typedef struct
{
	state_t states;
	out_t **func;
} transfunc_t;
status_t transfunc_init(transfunc_t **func, state_t num);
void transfunc_uninit(transfunc_t *func);
status_t transfunc_add_trans(transfunc_t *func, state_t state, char reading, trans_t *trans);
trans_t *transfunc_get_transition(transfunc_t *func, state_t state, char reading);

#endif // __TRANSITION_H__
