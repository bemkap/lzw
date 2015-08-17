CC     = gcc
CFLAGS = -Wall -pg

OBJDIR = obj
SRCDIR = src
BINDIR = bin

OBJECTS= $(addprefix $(OBJDIR)/,lzw.o encoder.o decoder.o list.o ltrie.o)

lzw: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(BINDIR)/lzw $(OBJECTS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	-rm $(OBJECTS) $(BINDIR)/lzw
