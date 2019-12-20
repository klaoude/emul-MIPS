NAME = emul-mips

CC = gcc

CFLAGS = -ggdb
LDFLAGS = 

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SOURCES = $(shell find $(SRC_DIR)/ -name "*.c")
HEADERS = $(shell find $(SRC_DIR)/ -name "*.h")
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

EXEC = $(BIN_DIR)/$(NAME)

all: $(EXEC)

clean:
	rm -rf bin/ obj/

$(EXEC): $(OBJECTS)
	mkdir -p $(@D)
	$(CC) -o $@ $^ $(LDFLAGS)

.SECONDEXPANSION:

$(OBJECTS): $$(patsubst $(OBJ_DIR)/%.o,$(SRC_DIR)/%.c,$$@)
	mkdir -p $(@D)
	$(CC) -o $@ -c $< $(CFLAGS)