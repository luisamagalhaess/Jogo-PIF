CC = gcc
FLAGS = -Wall -Wextra

CLILIB_SRC = $(HOME)/cli-lib/src
CLILIB_INC = $(HOME)/cli-lib/include

SRCS = alien.c defesa.c jogo.c scores.c main.c
CLILIB_SRCS = $(HOME)/cli-lib/src/keyboard.c $(HOME)/cli-lib/src/screen.c $(HOME)/cli-lib/src/timer.c

TARGET = jogo

all:
	$(CC) $(FLAGS) $(SRCS) $(CLILIB_SRCS) -I$(CLILIB_INC) -o $(TARGET)

clean:
	rm -f $(TARGET)