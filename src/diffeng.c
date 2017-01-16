#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "llvector.h"
#include "status.h"

/*
 * get_simulation_parameters - retrieves the first row and the number of columns
 * to calculate from the user
 * @param first - vector in which to store the first row
 * @param rows  - pointer in which to store the number of rows to calculate
 * @return - indication of success or failure
 */
status_t get_simulation_parameters(llvector_t *first, long long *rows);

/*
 * simulate_diff_engine - calculates the x and y values for the given number of
 * rows, including the provided one, printing them out as it goes, by
 * "simulating" the difference engine
 * @param row  - the first row, to be used to caclulate the subsequent rows
 * @param rows - the number of rows to output
 */
void simulate_diff_engine(llvector_t *row, long long rows);

/*
 * print_row_x_y - prints the 'x' and 'y' value for the given row, i.e., the
 * zeroth and first elements
 * @param row - the row for which to print the x and y values
 */
void print_row_x_y(llvector_t *row);

/*
 * to_long_long - convert the given str to a long long and store it in *value
 * @param str   - the string to convert
 * @param value - where to store the result
 * @return - indication of success or failure
 */
status_t to_long_long(char *str, long long *value);

int main(void)
{
	status_t error = SUCCESS;

	llvector_t *row = llvector_initialize();
	if (row == NULL)
	{
		error = OUT_OF_MEMORY;
		fprintf(stderr, "Ran out of memory\n");
		goto exit0;
	}

	long long rows;
	IF_ERROR_GOTO(get_simulation_parameters(row, &rows), "", exit1);
	simulate_diff_engine(row, rows);

exit1:
	llvector_uninitialize(row);
exit0:
	return error;
}

status_t get_simulation_parameters(llvector_t *first, long long *rows)
{
	status_t error = SUCCESS;
	char *line = NULL;
	size_t size;
	ssize_t chars_read;

	printf("Input the first row of differences:\n");
	while ((chars_read = getline(&line, &size, stdin)) > 1)
	{
		long long value;
		IF_ERROR_GOTO(to_long_long(line, &value), "", exit0);
		llvector_push_back(first, value);
	}

	CHECK_GETLINE(chars_read, exit0);

	printf("Input the number of rows to calculate:\n");
	chars_read = getline(&line, &size, stdin);
	CHECK_GETLINE(chars_read, exit0);
	IF_ERROR_GOTO(to_long_long(line, rows), "", exit0);
	printf("\n");

exit0:
	free(line);
	return error;

}

/*
 * LL_TABLE_ROW - returns a printf format string for a fixed-width two column
 * table of long long values, using its two parameters as the format specifiers
 * @param a - the format specifier for the left column
 * @param b - the format specifier for the right column
 * @return - format string for the types for the two given parameters
 */
#define LL_TABLE_ROW(a, b) ("%-19"a" %"b"\n")

void simulate_diff_engine(llvector_t *row, long long rows)
{
	printf(LL_TABLE_ROW("s", "s"), "x", "y");
	print_row_x_y(row);

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
		for (size_t col = 1; col < columns - 1; col++)
		{
			long long new_value = llvector_get(row, col) + llvector_get(row, col + 1);
			llvector_set(row, col, new_value);
		}

		print_row_x_y(row);
	}
}

void print_row_x_y(llvector_t *row)
{
	printf(LL_TABLE_ROW("lld", "lld"), llvector_get(row, 0), llvector_get(row, 1));
}

status_t to_long_long(char *str, long long *value)
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
