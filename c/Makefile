
SOURCES=main.c scanner.yy.c parser.tab.c slist.c

all: $(SOURCES)
	gcc -gdwarf-2 -obasic-s $(SOURCES) -lgc


scanner.yy.c: scanner.l
	flex -oscanner.yy.c scanner.l

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

test_ast: ast.h ast.c test_ast.c slist.h slist.c
	gcc -gdwarf-2 -otest_ast ast.c slist.c test_ast.c -lgc

test_slist: slist.h slist.c test_slist.c
	gcc -gdwarf-2 -otest_slist slist.c test_slist.c -lgc

clean:
	rm -f *.tab.*
	rm -f *.yy.c
	rm -f *.o
	rm -f basic-s
	rm -f test_ast
	rm -f test_slist

