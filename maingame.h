#ifndef _MAINGAME_H_
#define _MAINGAME_H_

#include "map.h"

const int playerCol[] = {
	Fore + Purple + Intense,
	Fore + Green + Intense,
	0, 0, 0
};
const char defName[][8] = {
	"Player0",
	"Player1",
	"Player2",
	"Player3",
	"Player4"
};


class mainGame{
	int playerCnt;
	player players[5];
	mapData *gameMap;
	public:
		mainGame();
		mainGame(int fidx);
		int loadMap(mapData *targetMap);
		void display();
		void refresh();
//		void regPlayer();
		int load(int fidx=0);
		int save(int fidx=0);
		int main();
};

#endif
