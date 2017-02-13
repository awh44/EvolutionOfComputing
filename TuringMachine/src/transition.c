#include <stdlib.h>
#include <string.h>

#include "transition.h"

#include "status.h"

struct out_t
{
	char reading;
	trans_t trans;
	out_t *next;
};

status_t transfunc_init(transfunc_t **func, state_t num)
{
	status_t error = SUCCESS;
	*func = malloc(sizeof(**func));
	if (*func == NULL)
	{
		error = OUT_OF_MEM;
		goto error0;
	}

	(*func)->func = malloc(num * sizeof(*(*func)->func));
	if ((*func)->func == NULL)
	{
		error = OUT_OF_MEM;
		goto error1;
	}

	memset((*func)->func, 0, num * sizeof(*(*func)->func));

	goto success;

error1:
	free(*func);
error0:

success:
	return error;
}

void transfunc_uninit(transfunc_t *func, state_t num)
{
	for (size_t i = 0; i < num; i++)
	{
		out_t *out = func->func[i];
		while (out != NULL)
		{
			out_t *next = out->next;
			free(out);
			out = next;
		}
	}

	free(func->func);
	free(func);
}

status_t transfunc_add_trans(transfunc_t *func, state_t state, char reading, trans_t *trans)
{
	status_t error = SUCCESS;

	out_t *out = malloc(sizeof(*out));
	if (out == NULL)
	{
		error = OUT_OF_MEM;
		goto exit0;
	}
	out->reading = reading;
	out->trans = *trans;
	out->next = func->func[state];
	func->func[state] = out;

exit0:
	return error;
}

trans_t *transfunc_get_transition(transfunc_t *func, state_t state, char reading)
{
	out_t *out = func->func[state];
	while ((out != NULL) && (out->reading != reading) && (out->reading != '_'))
	{
		out = out->next;
	}

	if (out == NULL)
	{
		return NULL;
	}

	return &out->trans;
}
