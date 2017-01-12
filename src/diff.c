#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "llvector.h"
#include "status.h"

status_t get_number(char *str, long long *value);
void print_row(llvector_t *row);

int main(void)
{
	status_t error = SUCCESS;

	llvector_t *row = llvector_initialize();
	if (row == NULL)
	{
		error = OUT_OF_MEMORY;
		goto exit0;
	}

	char *line = NULL;
	size_t size;
	ssize_t chars_read;

	printf("Input the first row of differences:\n");
	while ((chars_read = getline(&line, &size, stdin)) > 1)
	{
		long long value;
		IF_ERROR_GOTO(get_number(line, &value), "", exit1);
		llvector_push_back(row, value);
	}

	CHECK_GETLINE(chars_read, exit1);	

	printf("Input the number of rows to calculate:\n");
	chars_read = getline(&line, &size, stdin);
	CHECK_GETLINE(chars_read, exit1);
	long long rows;
	IF_ERROR_GOTO(get_number(line, &rows), "", exit1);
	printf("\n");

	// TODO: check the max possible value and length of a LLONG_MAX and specify the width
	printf("x				y\n");
	print_row(row);

	size_t columns = llvector_size(row);

	for (size_t i = 1; i < rows; i++)
	{
		//Increment the first column
		llvector_set(row, 0, llvector_get(row, 0) + 1);

		/*
			TODO: technically, calculating more values than are necessary - once the end of the
			table is being reached, columns start to stop being needed from the right.
		*/
		//Start at 1 because first column is the 'x' value
		//Stop at - 1 because the last column is constant
		for (size_t col = 1; col < llvector_size(row) - 1; col++)
		{
			long long new_value = llvector_get(row, col) + llvector_get(row, col + 1);
			llvector_set(row, col, new_value);
		}

		print_row(row);
	}

exit1:
	free(line);
	llvector_uninitialize(row);
exit0:
	return error;
}

status_t get_number(char *str, long long *value)
{
	status_t error = SUCCESS;

	char *end;
	*value = strtoll(str, &end, 10);
	if (*end != '\n')
	{
		error = INPUT_ERROR;
		fprintf(stderr, "Input was not a valid number.\n");
		goto exit0;
	}

exit0:
	return error;
}

void print_row(llvector_t *row)
{
	// TODO: check the max possible value and length of a LLONG_MAX and specify the width
	printf("%lld				%lld\n", llvector_get(row, 0), llvector_get(row, 1));
}
