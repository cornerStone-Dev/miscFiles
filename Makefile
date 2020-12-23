.PHONY: clean all

all: tool tool/exampleGram.o

tool:
	mkdir tool

tool/exampleGram.o: tool/exampleGram.c Makefile
	gcc -Os -c tool/exampleGram.c -o tool/exampleGram.o -Wall -Wextra
	size tool/exampleGram.o

tool/exampleGram.c: tool/lemon tool/exampleGram.y
	./tool/lemon tool/exampleGram.y -s -l -dtool

tool/lemon: tool/lemon.c tool/lempar.c
	gcc -O2 tool/lemon.c -o tool/lemon

tool/lemon.c:
	curl https://raw.githubusercontent.com/sqlite/sqlite/master/tool/lemon.c \
	> tool/lemon.c

tool/lempar.c:
	curl https://raw.githubusercontent.com/sqlite/sqlite/master/tool/lempar.c \
	> tool/lempar.c

#~ lempar_original.c:
#~ 	curl https://raw.githubusercontent.com/sqlite/sqlite/master/tool/lempar.c \
#~ 	> lempar.c


