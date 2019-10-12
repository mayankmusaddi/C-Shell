# Tutorial from http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =../lib

# LIBS=-lm

_DEPS = headers.h definitions.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o other.o redirection.o utilities.o
_LIB = builtin.o ls.o pinfo.o history.o setunset.o jobs.o kjob.o fgbg.o overkill.o cronjob.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ)) $(patsubst %,$(ODIR)/%,$(_LIB))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(LDIR)/%.c  $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

shell: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 