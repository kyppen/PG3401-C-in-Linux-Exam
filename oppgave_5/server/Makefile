# MakeFile from PG3401_Lecture_04_Pointers

INCLDIR = ./include
CC = gcc
CFLAGS = -O2
CFLAGS += -I$(INCLDIR)

OBJDIR = obj

_DEPS = famSend.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = server.o famSend.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -std=c89 -o $@ $< $(CFLAGS)

server: $(OBJS)
	$(CC) -std=c89 -o $@ $^ $(CFLAGS)

.PHONEY: clean
clean:
	rm -f ${OBJDIR)/*.o *~ core $(INCLDIR)/*~

