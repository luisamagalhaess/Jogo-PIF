CC = gcc
FLAGS = -Wall -Wextra

SRCS = alien.c defesa.c jogo.c scores.c main.c

TARGET = jogo

all:
	$(CC) $(FLAGS) $(SRCS) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o $(TARGET)

clean:
	rm -f $(TARGET)