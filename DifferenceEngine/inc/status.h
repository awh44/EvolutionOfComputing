#ifndef __STATUS_H__
#define __STATUS_H__

typedef enum
{
	SUCCESS = 0,
	OUT_OF_MEMORY,
	GETLINE_ERROR,
	INPUT_ERROR,
} status_t;

/*
 * IF_ERROR_GOTO - assigns the result of performing op to an "error" variable in the environment
 * and, if an error occcured, prints the given message to stderr, and goes to the provided label
 * @param op      - the operation to perform
 * @param message - the message to print
 * @param label   - the label to goto
 */
#define IF_ERROR_GOTO(op, message, label)\
	do\
	{\
		if ((error = (op)))\
		{\
			fprintf(stderr, "%s", message);\
			goto label;\
		}\
	} while (0)

/*
 * CHECK_GETLINE - checks chars_read, the return value from getline, and if an error occured,
 * sets an "error" variable in the environment appropriately, prints an error message, and goes
 * to the provided label
 * @param chars_read - value returned from getline
 * @param label      - the label to goto
 */
#define CHECK_GETLINE(chars_read, label)\
	do\
	{\
		if (chars_read < 0)\
		{\
			error = GETLINE_ERROR;\
			fprintf(stderr, "Error in retrieving line.\n");\
			goto label;\
		}\
	} while (0)

#endif
