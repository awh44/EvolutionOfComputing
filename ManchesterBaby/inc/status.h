#ifndef __STATUS_H__
#define __STATUS_H__

typedef enum
{
	SUCCESS,
	OUT_OF_MEM,
	INVALID_ADDRESS,
	PARSE_ERROR,
	FILE_OPEN_ERROR,
	UNRECOGNIZED_DIRECTIVE,
} status_t;

#endif // __STATUS_H__
