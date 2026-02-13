CC = gcc
CFLAGS = -Wall -Wextra -g
SOURCES = main.c execute.c parser.c builtins.c signals.c redirect.c
OBJ = $(SOURCES:.c=.o)
TARGET = jarvis
LDFLAGS = -lreadline

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
