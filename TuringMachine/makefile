CC=gcc
BLD=bld
SRC=src
INC=inc
IN=input
OUT=output

COMMON_FLAGS=-I$(INC) -std=gnu11 -Wall -o $@ $(DEBUG) $(MORE)
OBJ_FLAGS=$(COMMON_FLAGS) -c $^
PROG_FLAGS=$(COMMON_FLAGS) $^

PROG=turing
OBJS=$(BLD)/main.o $(BLD)/turing.o $(BLD)/transition.o $(BLD)/string_t.o

$(PROG): $(OBJS)
	$(CC) $(PROG_FLAGS)

$(OUT)/%.txt: $(IN)/%.txt $(PROG)
	./$(PROG) < $< > $@

$(BLD)/%.o: $(SRC)/%.c
	$(CC) $(OBJ_FLAGS)

.PHONY: clean
clean:
	rm -f $(BLD)/* $(PROG)
