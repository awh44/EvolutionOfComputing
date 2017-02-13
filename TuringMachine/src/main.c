#include <stdio.h>
#include <stdlib.h>

#include "transition.h"
#include "turing.h"
#include "status.h"

static unsigned short is_comment_line(char *line);
static status_t parse_turing_attributes(state_t *num, state_t *state, state_t *accept, state_t *reject, char *line);
static status_t parse_transition(transfunc_t *func, char *line);

int main(int argc, char **argv)
{
	status_t error = SUCCESS;
	if (argc != 3)
	{
		fprintf(stderr, "Usage: [machine configuration] [input tape]\n");
		error = ARGS_ERROR;
		goto exit0;
	}

	FILE *machine;
	if ((machine = fopen(argv[1], "r")) == NULL)
	{
		fprintf(stderr, "Error: Could not open %s\n", argv[1]);
		error = OPEN_ERROR;
		goto exit0;
	}

	FILE *tape;
	if ((tape = fopen(argv[2], "r")) == NULL)
	{
		fprintf(stderr, "Error: Could not open %s\n", argv[2]);
		error = OPEN_ERROR;
		goto exit1;
	}

	
	char *line = NULL;
	size_t len;
	ssize_t chars_read;

	do
	{
		chars_read = getline(&line, &len, machine);
	} while ((chars_read > 0) && is_comment_line(line));

	if (chars_read <= 0)
	{
		fprintf(stderr, "Error: Could not read Turing machine attributes\n");
		error = FILE_FORMAT_ERROR;
		goto exit2;
	}

	//Eliminate '\n'
	line[chars_read - 1] = '\0';
	state_t num, start, accept, reject;
	if ((error = parse_turing_attributes(&num, &start, &accept, &reject, line)))
	{
		fprintf(stderr, "Error: Could not parse Turing machine attributes %s\n", line);
		goto exit2;
	}

	turing_t *tm;
	if ((error = turing_init(&tm, num, start, accept, reject)))
	{
		goto exit2;
	}


	while ((chars_read = getline(&line, &len, machine)) > 0)
	{
		if (!is_comment_line(line))
		{
			//Eliminate '\n'
			line[chars_read - 1] = '\0';
			if ((error = parse_transition(tm->func, line)))
			{
				fprintf(stderr, "Error: Could not parse transition: %s\n", line);
				goto exit3;
			}
		}
	}

	//< 2 in this case because the tape must be at least one character long
	if ((chars_read = getline(&line, &len, tape)) < 2)
	{
		fprintf(stderr, "Error: Could not read tape input\n");
		error = FILE_FORMAT_ERROR;
		goto exit3;
	}

	//- 1 for the '\n'
	turing_set_tape(tm, line, chars_read- 1);

	if ((error = turing_run(tm)))
	{
		fprintf(stderr, "Error: Problem while running Turing machine\n");
		goto exit3;
	}

	turing_print_results(tm, stdout);

exit3:
	turing_uninit(tm);
exit2:
	free(line);
	fclose(tape);
exit1:
	fclose(machine);
exit0:
	return error;
}

static unsigned short is_comment_line(char *line)
{
	//'#' are comments, '\n' are empty lines - ignore
	return line[0] == '#' || line[0] == '\n';
}
static status_t parse_turing_attributes(state_t *num, state_t *state, state_t *accept, state_t *reject, char *line)
{
	status_t error = SUCCESS;
	
	char *start = line;
	char *end;

	*num = strtol(start, &end, 10);
	if (*end != ' ')
	{
		error = FILE_FORMAT_ERROR;
		goto exit0;
	}
	start = end + 1;

	*state = strtol(start, &end, 10);
	if (*end != ' ')
	{
		error = FILE_FORMAT_ERROR;
		goto exit0;
	}
	start = end + 1;

	*accept = strtol(start, &end, 10);
	if (*end != ' ')
	{
		error = FILE_FORMAT_ERROR;
		goto exit0;
	}
	start = end + 1;

	//Before we try to read the number, make sure there's a number there -
	//important in this case because *end is checked against '\0'
	if (*start == '\0')
	{
		error = FILE_FORMAT_ERROR;
		goto exit0;
	}

	*reject = strtol(start, &end, 10);
	if (*end != '\0')
	{
		error = FILE_FORMAT_ERROR;
		goto exit0;
	}

exit0:
	return error;
}

static status_t parse_transition(transfunc_t *func, char *line)
{
	status_t error = SUCCESS;

	char *start = line;
	char *end;

	//First thing is the number representing the state. Must be followed by a
	//space, and the string must not terminate after the space
	state_t state = strtol(start, &end, 10);
	if (end[0] != ' ' || end[1] == '\0')
	{
		error = FILE_FORMAT_ERROR;
		goto exit0;
	}
	//Skip the space
	start = end + 1;

	//After the space is the character being read
	char reading = *start;
	start++;

	//Must have a space following the read character
	if (*start != ' ')
	{
		error = FILE_FORMAT_ERROR;
		goto exit0;
	}
	start++;

	trans_t trans;

	//Next is number representing state to transition to
	trans.state = strtol(start, &end, 10);
	if (end[0] != ' ' || end[1] == '\0')
	{
		error = FILE_FORMAT_ERROR;
		goto exit0;
	}
	//Skip the space
	start = end + 1;

	//Read the character to write then go past it
	trans.write = *start;
	start++;

	//Space has to follow the written character
	if (*start != ' ')
	{
		error = FILE_FORMAT_ERROR;
		goto exit0;
	}
	//Skip the space
	start++;

	//Last thing on the line must be 'L' or 'R' for the head movement
	if (*start == 'L')
	{
		trans.dir = LEFT;
	}
	else if (*start == 'R')
	{
		trans.dir = RIGHT;
	}
	else
	{
		error = FILE_FORMAT_ERROR;
		goto exit0;
	}
	start++;

	//If the whole string hasn't been consumed, that's an error
	if (*start != '\0')
	{
		error = FILE_FORMAT_ERROR;
		goto exit0;
	}

	if ((error = transfunc_add_trans(func, state, reading, &trans)))
	{
		goto exit0;
	}

exit0:
	return error;
}
