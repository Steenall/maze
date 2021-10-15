SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

EXE := $(BIN_DIR)/game
DEBUGEXE := $(BIN_DIR)/debug
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC = gcc -O

CFLAGS   := -Iinclude -Wextra -ansi -pedantic
LDFLAGS  := -Llib
LDLIBS   := -lm
DEBUGFLAGS := -ggdb -DDEBUG -DSTEPBYSTEP 

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

debug: $(OBJ) | $(BIN_DIR)
	$(CC) $(DEBUGFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $(DEBUGEXE)
	
$(OBJ_DIR)/*-d.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)
