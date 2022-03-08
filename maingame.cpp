#include "maingame.h"

char* player::getDesc(char *Desc, int idx){
	sprintf(Desc, "%s: hp:%d level:%d speed:%d score:%d\n", Name, idx, hp, lvl, sco, spd);
	return Desc;
}

void mainGame::display(){
	for(int i = 0; i < playerCnt; i++){
		char Desc[505];
		printf("%s", players[i].getDesc(Desc));
	}
}


