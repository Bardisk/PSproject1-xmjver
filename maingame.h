#ifndef _MAINGAME_H_
#define _MAINGAME_H_

#include "map.h"

class mainGame{
	int playerCnt;
	player players[5];
	mapData *gameMap;
	public:
		mainGame();
		mainGame(int fidx);
		void display();
		int load(int fidx=0);
		int save(int fidx=0);
		int main();
};

#endif
