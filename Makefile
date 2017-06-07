CC=g++
CFLAGS=-O2 -Wall -std=c++11 -Wno-missing-braces
LFLAGS=-lraylib -lopengl32 -lraylib -lglfw3 -lgdi32

SOURCES=main.cpp

all:
	$(CC) $(SOURCES) $(CFLAGS) $(LFLAGS) -o ray.exe

debug:
	$(CC) -DDEBUG $(SOURCES) $(CFLAGS) $(LFLAGS) -o ray.exe

clean:
	del ray.exe
