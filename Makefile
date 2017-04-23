CC      = /usr/bin/cc
CFLAGS  = -std=c99 -Wall
LDFLAGS = 

SRCDIR = src
OBJDIR = obj

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
EXE = loop

.PHONY: clean

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

$(OBJ): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) -c -o $@ $(CFLAGS) $<

clean:
	rm -f $(OBJ) $(EXE)
