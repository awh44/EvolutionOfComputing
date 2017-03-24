CC=gcc
BLD=bld
SRC=src
INC=inc
IN=input

COMMON_FLAGS=-I$(INC) -std=gnu11 -Wall -o $@ $(DEBUG) $(MORE)
OBJ_FLAGS=$(COMMON_FLAGS) -c $^
PROG_FLAGS=$(COMMON_FLAGS) $^

PROG=baby
OBJS=$(BLD)/main.o $(BLD)/baby.o

$(PROG): $(OBJS)
	$(CC) $(PROG_FLAGS)

$(BLD)/%.o: $(SRC)/%.c
	$(CC) $(OBJ_FLAGS)

.PHONY: clean
clean:
	rm -f $(BLD)/* $(PROG)
