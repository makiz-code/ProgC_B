CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
SRCDIR = src
BINDIR = .
SOURCES = $(SRCDIR)/main.c $(SRCDIR)/books.c $(SRCDIR)/loans.c $(SRCDIR)/stats.c $(SRCDIR)/utils.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = bibliotheque

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(BINDIR)/$(TARGET) $(SOURCES)

clean:
	rm -f $(BINDIR)/$(TARGET) $(OBJECTS)

