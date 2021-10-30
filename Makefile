SRC_DIR := src
OBJ_DIR := obj
OBJ_DEBUG_DIR := objDebug
BIN_DIR := bin
TEST_DIR = $(SRC_DIR)/test

EXECUTABLE_SOURCES := $(SRC_DIR)/main.c
TESTSUITE_SOURCES := $(TEST_DIR)/test_eratosthenes.c

EXE := $(BIN_DIR)/game
DEBUGEXE := $(BIN_DIR)/debug
SRC := $(wildcard $(SRC_DIR)/*.c)
SRC_TEST := $(filter-out $(TESTSUITE_SOURCES) $(EXECUTABLE_SOURCES), $(SRC))
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJ_DEBUG := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DEBUG_DIR)/%.o)

CC = gcc -O

CFLAGS   := -Iinclude -Wextra -ansi -pedantic
LDFLAGS  := -Llib
LDLIBS   := -lm
DEBUGFLAGS := -ggdb -DDEBUG -DSTEPBYSTEP 

.DEFAULT:
	echo "$< n'existe pas"

.PHONY: all clean mrproper debug distclean

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR) $(OBJ_DEBUG_DIR):
	mkdir -p $@

debug: $(OBJ_DEBUG) | $(BIN_DIR)
	$(CC) $(DEBUGFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $(DEBUGEXE)
	
$(OBJ_DEBUG_DIR)/%.o: $(SRC)/%.c | $(OBJ_DEBUG_DIR)
	$(CC) $(CFLAGS) $(CFLAGS) $(DEBUGFLAGS) -c $< -o $@

clean:
	@$(RM) -rv $(OBJ_DIR) $(OBJ_DEBUG_DIR)

mrproper:
	@$(RM) -rv $(BIN_DIR)

distclean: clean mrproper
