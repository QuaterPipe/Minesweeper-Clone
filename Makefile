all: Minesweeper run
CC = g++
CCFLAGS = -Wall -Werror -O3 -std=c++17
LINKER_FLAGS = -L. -lmingw32 -lcomctl32 -lSDL2main -lSDL2 -L./lib -lSDL2_image

o/%.o : src/%.cpp
	$(CC) $(CCFLAGS) -c -o $@ $<

o/info.res : src/info.rc
	windres src/info.rc -O coff -o o/info.res

clear:
	rm o/*.o

compRelease:
	$(CC) -mwindows o/*.o -o Minesweeper  -L. -lSDL2 -lmingw32 -lcomctl32 -L./lib -lSDL2main -lSDL2_image -static -static-libstdc++ -L. -lgcc -lpthread o/info.res

Minesweeper: o/Board.o o/Game.o o/LoadMedia.o o/Stats.o o/main.o o/Texture.o o/info.res
	$(CC) $(CCFLAGS) -mwindows o/*.o -o Minesweeper $(LINKER_FLAGS) o/info.res
# -mwindows
run:
	./Minesweeper