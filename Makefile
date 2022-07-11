build:
	gcc atomic.c lexer.c parser.c objects.c runtime.c stl.c -Wall -Og -g

release:
	gcc atomic.c lexer.c parser.c objects.c runtime.c stl.c -Os -Wall

debug: build
	gdb a.out --silent
