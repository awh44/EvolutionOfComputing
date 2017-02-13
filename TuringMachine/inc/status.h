#ifndef __STATUS_H__
#define __STATUS_H__

typedef enum
{
	SUCCESS = 0,
	OUT_OF_MEM,
	OPEN_ERROR,
	ARGS_ERROR,
	READ_ERROR,
	INVALID_TRANS,
	FILE_FORMAT_ERROR,
} status_t;

#endif // __STATUS_H__
