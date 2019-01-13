all: ttysort

CC=gcc

LIBS=-ltermbox
CFLAGS=

FLAGS=-O3 -pipe -s -ansi
DEBUGFLAGS=-Og -pipe -g

INPUT=ttysort.c
OUTPUT=ttysort

INSTALLDIR=$(HOME)/.local/bin

RM=/bin/rm

ttysort:
	$(CC) $(INPUT) -o $(OUTPUT) $(LIBS) $(CFLAGS) $(FLAGS)

debug:
	$(CC) $(INPUT) -o $(OUTPUT) $(LIBS) $(CFLAGS) $(DEBUGFLAGS)

install:
	test -d $(INSTALLDIR) || mkdir -p $(INSTALLDIR)
	install -pm 755 $(OUTPUT) $(INSTALLDIR)

clean:
	if [ -e $(OUTPUT) ]; then $(RM) $(OUTPUT); fi
