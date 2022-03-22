#include "maingame.h"

extern char buff[4096];
extern int tickCntSinceLoaded;
extern clock_t lastRespondTime, lastLoadedTime;
extern bool isRealTimeMode;

mainGame::mainGame(){
	gameMap = NULL;
	playerCnt = 2;
	keyCatcher p1('W','S','D','A',' '),p2(VK_UP,VK_DOWN,VK_RIGHT,VK_LEFT,VK_OEM_2);
	players[0] = player(defName[0], '$', playerCol[0], cursor(1,1), p1);
	players[1] = player(defName[1], '$', playerCol[1], cursor(13,15), p2);
}

int mainGame::loadMap(mapData *targetMap){
	if (targetMap->check()) {
		putr("Invaild Map");
		return -1;
	}
	gameMap = targetMap;
	for (int i = 0; i < playerCnt; i++) {
		players[i].pos.renew(targetMap);
		players[i].myMap = targetMap;
	}
	return 0;
}
mainGame::mainGame(int fidx){
	if(!~load(fidx)){
		printw("Load Failure!\n");
	}
}

int mainGame::load(int fidx){
	char fName[20];
	strcpy(fName, RAWSNAME);
	fName[8] = fidx + '0';
	FILE *fp = fopen(fName, "rb");
	if (!fp) {
		printw("WARNING: %s is not found.\n", fName);
		return -1;
	}
	printw("Reading Map Data...\n");
	gameMap->szN = gameMap->szM = 0;
	memset(gameMap->mapbuf, 0, sizeof gameMap->mapbuf);
	fread(&gameMap->szN, sizeof (int), 1, fp);
	fread(&gameMap->szM, sizeof (int), 1, fp);
	fread(gameMap->mapbuf, sizeof (node), gameMap->szN*gameMap->szM, fp);
	printw("Registering the players...\n");
	playerCnt = 0;
	fread(&playerCnt, sizeof (int), 1, fp);
	fread(players, sizeof (player), playerCnt, fp);
	printw("Loading Map for players...\n");
	if(!~loadMap(gameMap)){
		putr("ERROR: MAP IS INVAILD!");
		return -1;
	}
	printw("Everything Done!\n");
	fclose(fp);
	return 0;
}
int mainGame::save(int fidx){
	char fName[20];
	strcpy(fName, RAWSNAME);
	fName[8] = fidx + '0';
	FILE *fp = fopen(fName, "wb");
	if (!fp) {
		printw("WARNING: %s is not found.\n", fName);
		return -1;
	}
	printw("Saving Map Data...\n");
	fwrite(&gameMap->szN, sizeof (int), 1, fp);
	fwrite(&gameMap->szM, sizeof (int), 1, fp);
	fwrite(gameMap->mapbuf, sizeof (node), gameMap->szN*gameMap->szM, fp);
	printw("Saving the players...\n");
	fwrite(&playerCnt, sizeof (int), 1, fp);
	fwrite(players, sizeof (player), playerCnt, fp);
	printw("Everything Done!\n");
	fclose(fp);
	return 0;
}

void mainGame::display(){
	for(int i = 0; i < playerCnt; i++){
		char Desc[505];
		printf("%s", players[i].getDesc(Desc));
	}
}

void mainGame::refresh(){
	cls();
	for (int i = 0; i < playerCnt; i++) {
		char tmpdesc[100];
		players[i].getDesc(tmpdesc);
		printf(tmpdesc);
	}
	drawSettings dss;
	for (int i = 0; i < playerCnt; i++)
		dss.addPlayer(&players[i]);
	gameMap->draw(dss);
	fflush(stdout);
	return ;
}

void mainGame::realTimeDealing(){
	for (int i = 0; i < playerCnt; i++)
		if (players[i].movCnt) --players[i].movCnt;
	for (int i = 0; i < gameMap->szN * gameMap->szM; i++) {
		if (gameMap->mapbuf[i].type & BOMB) {
			gameMap->mapbuf[i].type -= (1 << 24);
			if (!(gameMap->mapbuf[i].type >> 24)){
				gameMap->mapbuf[i].type ^= BOMB;
				//to-do
				reflag = true;
			}
		}
	}
	return ;
}

int mainGame::main(){
	FILE *flog = fopen("realtimelog.txt", "w");
	fprintf(flog, "%lld\n", time(0));
	cls();
	
	SenterRealTime();
	hideCursor();
	enterRealTime();
	refresh();
	while (true) {
		clock_t nowt = clock();
		if (nowt - lastRespondTime < GAMETICK)
			continue;
		++tickCntSinceLoaded;
		fprintf(flog, "tick: %d, timegap: %ld\n", tickCntSinceLoaded, nowt - lastRespondTime);
		lastRespondTime = nowt;
		realTimeDealing();
		if (_kbhit()){
			if(GetKeyState(VK_ESCAPE) & 0x8000){
				cls();
				puts("");
				exitRealTime();
				fclose(flog);
				SexitRealTime(); 
				showCursor();
				return 0;
			}
			for(int i = 0; i < playerCnt; i++){
				if (players[i].movCnt) continue;
				if (~ players[i].keyCatch.dealInput(' ', &players[i])) {
					players[i].movCnt = players[i].spd;
					reflag |= 1;
				}
			}
		}
		reflag ? (refresh()) : (void) 0;
		reflag = false;
//		reflag ? (reflag=refresh(bsflag)) : 0;
	}
	fclose(flog);
	exitRealTime();
	showCursor();
	SexitRealTime(); 
	return 0;	
}


