all:
	g++ -g -I include/ -L lib/ -o main src/main.cpp src/GameObjects.cpp src/Game.cpp src/GUI.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf