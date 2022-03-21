#include "maingame.h"

extern char buff[4096];
extern int tickCntSinceLoaded;
extern clock_t lastRespondTime, lastLoadedTime;
extern bool isRealTimeMode;

mainGame::mainGame(){
	gameMap = NULL;
	playerCnt = 2;
	players[0] = player(defName[0], '$', playerCol[0], cursor(1,1));
	players[1] = player(defName[1], '$', playerCol[1], cursor(13,15));
}

int mainGame::loadMap(mapData *targetMap){
	if (targetMap->check()) {
		putr("Invaild Map");
		return -1;
	}
	gameMap = targetMap;
	for (int i = 0; i < playerCnt; i++)
		players[i].pos.renew(targetMap);
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


int mainGame::main(){
	FILE *flog = fopen("realtimelog.txt", "w");
	fprintf(flog, "%lld\n", time(0));
	cls();
	
	SenterRealTime();
	hideCursor();
	enterRealTime();
	bool reflag = false;
	refresh();
	while (true) {
		clock_t nowt = clock();
		if (nowt - lastRespondTime < GAMETICK)
			continue;
		++tickCntSinceLoaded;
		fprintf(flog, "tick: %d, timegap: %ld\n", tickCntSinceLoaded, nowt - lastRespondTime);
		lastRespondTime = nowt;

		if (_kbhit()){
			char ch = _getch();
			switch (ch) {
				case 0x1b: //esc
					cls();
					puts("Exit from edit mode.");
					exitRealTime();
					fclose(flog);
					SexitRealTime(); 
					showCursor();
					return 0;
				case '\r':
					break;
				default:
					reflag |= 1;
			}
		}
//		reflag ? (reflag=refresh(bsflag)) : 0;
	}
	fclose(flog);
	exitRealTime();
	showCursor();
	SexitRealTime(); 
	return 0;	
}


