CC = gcc
FLAGS = -Wall -Wextra

SRCS = src/alien.c src/defesa.c src/jogo.c src/scores.c src/main.c

TARGET = jogo

UNAME := $(shell uname 2>/dev/null || echo Windows)

ifeq ($(UNAME), Linux)
    LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

ifeq ($(UNAME), Darwin)
    LIBS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo
endif

ifeq ($(UNAME), Windows)
    LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
    TARGET = jogo.exe
endif

all:
	$(CC) $(FLAGS) $(SRCS) -I include/ $(LIBS) -o $(TARGET)

clean:
	rm -f jogo jogo.exe