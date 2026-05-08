CC = gcc
FLAGS = -Wall -Wextra

SRCS = src/alien.c src/defesa.c src/jogo.c src/scores.c src/main.c

TARGET = jogo

all:
	$(CC) $(FLAGS) $(SRCS) -I include/ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o $(TARGET)

clean:
	rm -f $(TARGET)