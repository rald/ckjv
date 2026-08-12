bibly: main.c common.h strutil.h util.h info.h token.h lexer.h parser.h cite.h
	gcc main.c -o bibly -lm -g

clean:
	rm bibly
