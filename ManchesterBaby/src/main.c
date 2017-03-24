#include <stdio.h>
#include <stdlib.h>

#include "baby.h"
#include "status.h"

status_t process_input_file(baby_t *baby, char *filename);
status_t process_line(baby_t *baby, char *line, ssize_t chars_read);
status_t process_load(baby_t *baby, char *line);
status_t process_current(baby_t *baby, char *line);
status_t process_execute(baby_t *baby, char *line);
status_t process_print(baby_t *baby, char *line);

int main(int argc, char **argv)
{
	status_t error = SUCCESS;

	baby_t *baby;
	if ((error = baby_init(&baby)))
	{
		fprintf(stderr, "Error: could not initialize\n");
		goto exit0;
	}

	char *line = NULL;
	size_t len = 0;
	ssize_t chars_read;
	
	if (argc > 1)
	{
		process_input_file(baby, argv[1]);
	}

	printf("> ");
	while ((chars_read = getline(&line, &len, stdin)) > 0)
	{
		process_line(baby, line, chars_read);
		printf("> ");
	}
	printf("\n");

	free(line);
	baby_uninit(baby);
exit0:
	return error;
}

status_t process_input_file(baby_t *baby, char *filename)
{
	status_t error = SUCCESS;

	FILE *input;
	if ((input = fopen(filename, "r")) == NULL)
	{
		error = FILE_OPEN_ERROR;
		goto exit0;
	}

	char *line = NULL;
	size_t len = 0;
	ssize_t chars_read;
	while ((chars_read = getline(&line, &len, input)) > 0)
	{
		printf("Processing line: %s", line);
		if ((error = process_line(baby, line, chars_read)))
		{
			goto exit1;
		}
	}

exit1:
	fclose(input);
exit0:
	return error;
}

status_t process_line(baby_t *baby, char *line, ssize_t chars_read)
{
	status_t error = SUCCESS;
	// Eliminate the '\n'
	chars_read--;
	line[chars_read] = '\0';

	switch (line[0])
	{
		// Skip blank lines and comments
		case '\0':
		case '#':
			break;
		case 'l':
			error = process_load(baby, line);
			break;
		case 'c':
			error = process_current(baby, line);
			break;
		case 'e':
			error = process_execute(baby, line);
			break;
		case 'd':
			error = process_print(baby, line);
			break;
		default:
			fprintf(stderr, "Unrecognized directive: %s", line);
			error = UNRECOGNIZED_DIRECTIVE;
			break;
	}

	return error;
}

status_t parse_load(char *line, uint32_t *addr, int32_t *value)
{
	status_t error = SUCCESS;
	// Start after the 'l'
	char *start = line + 1;
	if (*start != ' ')
	{
		error = PARSE_ERROR;
		goto exit;
	}
	// Consume space
	start++;

	int base = 10;
	if (start[0] == '0')
	{
		if (start[1] == 'x')
		{
			base = 16;
			start += 2;
		}
		else if (start[1] == 'b')
		{
			base = 2;
			start += 2;
		}
	}

	char *end;
	*addr = strtoul(start, &end, base);
	if ((end[0] != ' ') || (end[1] == '\0'))
	{
		error = PARSE_ERROR;
		goto exit;
	}
	// Skip space
	start = end + 1;

	base = 10;
	if (start[0] == '0')
	{
		if (start[1] == 'x')
		{
			base = 16;
			start += 2;
		}
		else if (start[1] == 'b')
		{
			base = 2;
			start += 2;
		}
	}
	
	*value = strtol(start, &end, base);
	if (*end != '\0')
	{
		error = PARSE_ERROR;
		goto exit;
	}

exit:
	return error;
}

status_t process_load(baby_t *baby, char *line)
{
	status_t error = SUCCESS;

	uint32_t addr;
	int32_t value;
	if ((error = parse_load(line, &addr, &value)))
	{
		fprintf(stderr, "Error: could not parse load directive.\n");
		goto exit;
	}

	if ((error = baby_load_value(baby, addr, value)))
	{
		fprintf(stderr, "Error: could not load value to that address.\n");
		goto exit;
	}

exit:
	return error;
}

status_t parse_current(char *line, uint32_t *addr)
{
	status_t error = SUCCESS;
	// Start after the 'c'
	char *start = line + 1;
	if (*start != ' ')
	{
		error = PARSE_ERROR;
		goto exit;
	}
	// Consume space
	start++;

	int base = 10;
	if (start[0] == '0')
	{
		if (start[1] == 'x')
		{
			base = 16;
			start += 2;
		}
		else if (start[1] == 'b')
		{
			base = 2;
			start += 2;
		}
	}

	char *end;
	*addr = strtoul(start, &end, base);
	if (*end != '\0')
	{
		error = PARSE_ERROR;
		goto exit;
	}

exit:
	return error;
}

status_t process_current(baby_t *baby, char *line)
{
	status_t error = SUCCESS;

	uint32_t addr;
	if ((error = parse_current(line, &addr)))
	{
		fprintf(stderr, "Error: could not parse current instruction directive.\n");
		goto exit;
	}

	if ((error = baby_load_current_instruction(baby, addr)))
	{
		fprintf(stderr, "Error: could not set the current instruction to that address.\n");
		goto exit;
	}
exit:
	return error;
}

status_t parse_execute(char *line, uint32_t *num_steps)
{
	status_t error = SUCCESS;
	char *start = line + 1;
	if (*start != ' ')
	{
		error = PARSE_ERROR;
		goto exit;
	}
	// Consume space
	start++;

	char *end;
	*num_steps = strtoul(start, &end, 10);
	if (*end != '\0')
	{
		error = PARSE_ERROR;
		goto exit;
	}

exit:
	return error;
}

status_t process_execute(baby_t *baby, char *line)
{
	status_t error = SUCCESS;
	// If the only thing on the line is the 'e', just execute until a
	// halt instruction is encountered
	if (line[1] == '\0')
	{
		if ((error = baby_execute(baby)))
		{
			fprintf(stderr, "Error: issue executing present instruction.\n");
		}
		goto exit;
	}

	uint32_t num_steps;
	if ((error = parse_execute(line, &num_steps)))
	{
		fprintf(stderr, "Error: could not parse execute directive.\n");
		goto exit;
	}

	if ((error = baby_execute_num_steps(baby, num_steps)))
	{
		fprintf(stderr, "Error: issue executing present instruction.\n");
		goto exit;
	}

exit:
	return error;
}

status_t process_print(baby_t *baby, char *line)
{
	baby_display(baby, stdout);
	return SUCCESS;
}
