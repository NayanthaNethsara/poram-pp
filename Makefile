CC = gcc
LEX = flex
YACC = bison

SRC_DIR = src
BIN_DIR = output
TARGET = poram

LEX_FILE = $(SRC_DIR)/lexer.l
YACC_FILE = $(SRC_DIR)/parser.y
MAIN_FILE = $(SRC_DIR)/main.c
GEN_FILE = $(SRC_DIR)/generator.c
MODEL_FILE = $(SRC_DIR)/model.c

OBJS = parser.tab.c lex.yy.c

CFILES = $(MAIN_FILE) $(GEN_FILE) $(MODEL_FILE)

INCLUDES = -Isrc

all: $(TARGET)

$(TARGET): $(OBJS) $(CFILES)
	$(CC) -o $(TARGET) $(OBJS) $(CFILES) $(INCLUDES) -lfl

parser.tab.c parser.tab.h: $(YACC_FILE)
	$(YACC) -d $(YACC_FILE) -o parser.tab.c

lex.yy.c: $(LEX_FILE)
	$(LEX) $(LEX_FILE)

exe: $(OBJS) $(CFILES)
	x86_64-w64-mingw32-gcc -o $(TARGET).exe $(OBJS) $(CFILES) $(INCLUDES)

run: all
	./$(TARGET) < examples/registration.poram

run-complex: all
	./$(TARGET) < examples/complex.poram

clean:
	rm -f parser.tab.* lex.yy.c $(TARGET) $(TARGET).exe $(BIN_DIR)/*.html

.PHONY: all clean run exe run-complex
