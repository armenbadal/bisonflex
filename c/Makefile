
SOURCES=main.c scanner.yy.c parser.tab.c slist.c

all: $(SOURCES)
	gcc -gdwarf-2 -obasic-s $(SOURCES) -lgc


scanner.yy.c: scanner.l
	flex -oscanner.yy.c scanner.l

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

test_ast:
	gcc -otest_ast ast.c test_ast.c

clean:
	rm -f *.tab.*
	rm -f *.yy.c
	rm -f *.o
	rm -f basic-s
	rm -f test_ast

