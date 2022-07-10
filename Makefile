build:
	gcc atomic.c lexer.c parser.c objects.c -Wall -Og -g

release:
	gcc atomic.c lexer.c parser.c objects.c -Os -Wall

debug: build
	gdb a.out --silent
