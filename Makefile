CC = gcc
LEX = flex
YACC = bison

SRC_DIR = src
BIN_DIR = output
TARGET = formlang

LEX_FILE = $(SRC_DIR)/lexer.l
YACC_FILE = $(SRC_DIR)/parser.y
MAIN_FILE = $(SRC_DIR)/main.c
GEN_FILE = $(SRC_DIR)/generator.c

OBJS = parser.tab.c lex.yy.c

all: $(TARGET)

$(TARGET): $(OBJS) $(MAIN_FILE) $(GEN_FILE)
	$(CC) -o $(TARGET) $(OBJS) $(MAIN_FILE) $(GEN_FILE) -Isrc -lfl

parser.tab.c parser.tab.h: $(YACC_FILE)
	$(YACC) -d $(YACC_FILE) -o parser.tab.c

lex.yy.c: $(LEX_FILE)
	$(LEX) $(LEX_FILE)

run: all
	./$(TARGET) < examples/registration.form
	
run-complex: all
	./$(TARGET) < examples/complex.form

clean:
	rm -f parser.tab.* lex.yy.c $(TARGET) $(BIN_DIR)/*.html

.PHONY: all clean run
