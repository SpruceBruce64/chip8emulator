OBJS = main.cpp

#Exec name
OBJ_NAME = chip8emu


all: $(OBJS) chip8.o
	gcc $(OBJS) chip8.o -lSDL2 -lstdc++ -o $(OBJ_NAME)

emu.o : chip8.cpp chip8.h
	gcc -c chip8.cpp	

