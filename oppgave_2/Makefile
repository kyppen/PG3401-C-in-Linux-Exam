# MakeFile from PG3401_Lecture_04_Pointers

INCLDIR = ./include
CC = gcc
CFLAGS = -O2
CFLAGS += -I$(INCLDIR)

OBJDIR = obj

_DEPS = eksamen_v24_oppgave2_all_lower.h eksamen_v24_oppgave2_all_upper.h eksamen_v24_oppgave2_anagram.h eksamen_v24_oppgave2_disjoint.h eksamen_v24_oppgave2_heterogram.h eksamen_v24_oppgave2_palindrom.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = Oppgave2.o eksamen_v24_oppgave2_all_lower.o eksamen_v24_oppgave2_all_upper.o eksamen_v24_oppgave2_anagram.o eksamen_v24_oppgave2_disjoint.o eksamen_v24_oppgave2_heterogram.o eksamen_v24_oppgave2_palindrom.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -std=c89 -o $@ $< $(CFLAGS)

oppgave2: $(OBJS)
	$(CC) -std=c89 -o $@ $^ $(CFLAGS)

.PHONEY: clean
clean:
	rm -f ${OBJDIR)/*.o *~ core $(INCLDIR)/*~

