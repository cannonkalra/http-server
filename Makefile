IDIR =../include

CC=clang
CFLAGS=-I$(IDIR) -g -Wall -gdwarf-4

ODIR=.
LDIR =../lib

LIBS=-lm

_DEPS = http_server.h webserver.h request.h io_helper.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = http_server.o webserver.o request.o io_helper.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: http_server

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

http_server: $(OBJ) webserver.o
	$(CC) -o $@.out $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
