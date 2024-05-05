IDIR =../include

CC=clang
CFLAGS=-I$(IDIR) -g -Wall

ODIR=.
LDIR =../lib

LIBS=-lm

_DEPS = handler.h pool.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = server.o handler.o pool.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: server

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server: $(OBJ) server.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ $(ODIR)/*.out
