CC = gcc
FLAGS = -Wall -Wextra

SRCS = src/alien.c src/defesa.c src/jogo.c src/scores.c src/main.c

UNAME := $(shell uname 2>/dev/null)

ifneq (,$(findstring MINGW,$(UNAME)))
    LIBS = -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
    TARGET = jogo.exe
else ifeq ($(UNAME), Darwin)
    LIBS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo
    TARGET = jogo
else
    LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    TARGET = jogo
endif

all:
	$(CC) $(FLAGS) $(SRCS) -I include/ $(LIBS) -o $(TARGET)

clean:
	rm -f jogo jogo.exe