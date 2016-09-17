

all: main.c scanner.yy.c parser.tab.c slist.c
	gcc -gdwarf-2 -obasic-s main.c scanner.yy.c parser.tab.c slist.c


scanner.yy.c: scanner.l
	flex -oscanner.yy.c scanner.l

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y


clean:
	rm -f *.tab.*
	rm -f *.yy.c
	rm -f *.o
	rm -f basic-s

