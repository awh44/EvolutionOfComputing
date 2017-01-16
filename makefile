CC=gcc
BLD=bld
SRC=src
INC=inc/

COMMON_FLAGS=-I$(INC) -Wall -o $@ $(DEBUG) $(MORE)
OBJ_FLAGS=$(COMMON_FLAGS) -c $^
PROG_FLAGS=$(COMMON_FLAGS) $^

OBJS=$(BLD)/diff.o $(BLD)/llvector.o

diff: $(OBJS)
	$(CC) $(PROG_FLAGS)

$(BLD)/%.o: $(SRC)/%.c
	$(CC) $(OBJ_FLAGS)
