#include "maingame.h"


void mainGame::display(){
	for(int i = 0; i < playerCnt; i++){
		char Desc[505];
		printf("%s", players[i].getDesc(Desc));
	}
}


