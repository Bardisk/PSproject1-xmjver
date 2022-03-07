#ifndef _MAINGAME_H_
#define _MAINGAME_H_

#include "map.h"

struct player{
	pair pos;
	int spd, hp, lvl;
	std::string Name;
};

class mainGame{
	int playerCnt;
	player players[5];
	public:
		mainGame();
		mainGame(int fidx);
		void display();
		int load(int fidx=0);
		int save(int fidx=0);
};

#endif
