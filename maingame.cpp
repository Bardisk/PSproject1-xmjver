#include "maingame.h"
#include "ai.h"
#include <iostream>

extern char buff[4096];
extern int tickCntSinceLoaded;
extern clock_t lastRespondTime, lastLoadedTime;
extern bool isRealTimeMode;

mainGame::mainGame(){
	gameMap = NULL;
	playerCnt = 4;
	keyCatcher p1('W','S','D','A',' '), p2(VK_UP,VK_DOWN,VK_RIGHT,VK_LEFT,VK_OEM_2);
	players[0] = player(defName[0], '$', playerCol[0], cursor(1,1), p1, NULL, 0);
	players[1] = player(defName[1], '$', playerCol[1], cursor(13,15), p2, NULL, 1);
	players[2] = player(defName[2], '&', playerCol[2], cursor(1,15), p1, NULL, 2);
	players[2].isAI = true;
	players[2].robot = new AI();
	players[3] = player(defName[3], '&', playerCol[3], cursor(13,1), p1, NULL, 3);
	players[3].isAI = true;
	players[3].robot = new AI();
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
	printf("\033[36;4mfps:\033[0m %.1lf (%.1lf locked)\n", 1000.0/(clock() - lastLoadedTime)*(double) tickCntSinceLoaded, 1000.0/GAMETICK);
	for (int i = 0; i < playerCnt; i++) {
		char tmpdesc[100];
		players[i].getDesc(tmpdesc);
		printf(tmpdesc);
	}
	drawSettings dss;
	for (int i = 0; i < playerCnt; i++){
		if (players[i].hp <= 0) continue;
		dss.addPlayer(&players[i]);
	}
	gameMap->draw(dss);
	puts("ESC - Exit \t P - Pause");
	fflush(stdout);
	return ;
}

int mainGame::realTimeDealing(){
	int aliveCnt = 0;
	if (tickCntSinceLoaded % (1000/GAMETICK) == 0) reflag = true;
	for (int i = 0; i < playerCnt; i++) {
		if (players[i].movCnt) --players[i].movCnt;
		if (players[i].hp > 0) {
			++aliveCnt;
			if (tickCntSinceLoaded % (1000/GAMETICK) == 0)
				players[i].sco += SCO_PER_SEC;
			aliveP = i;
		}
	}
	if (aliveCnt == 1) {
		return -1;
	}
	if (aliveCnt == 0) {
		return -2;
	}
	for (int i = 0; i < gameMap->szN * gameMap->szM; i++) {
		nodeInfo tmp=gameMap->mapbuf[i].getInfo();
		if (gameMap->mapbuf[i].type & BOMB) {
			gameMap->mapbuf[i].type -= (1 << 24);
			if (!(gameMap->mapbuf[i].type >> 24)){
				gameMap->triggerBomb(cursor(i/gameMap->szM, i%gameMap->szM, gameMap->szN, gameMap->szM), &players[(int) tmp.info.bvalue.owner]);
				//to-do
				reflag = true;
			}
			continue;
		}
		if (gameMap->mapbuf[i].type & WAVE){
			for (int j = 0; j < playerCnt; j++) {
				if (players[j].pos.calNum() == i){
					if (players[j].hp) {
						reflag = true;
						if(j != tmp.info.bvalue.owner)
							players[(int) tmp.info.bvalue.owner].sco += SCO_PER_HP;
						--players[j].hp;
					}
				}
			}
			gameMap->mapbuf[i].type -= (1 << 24);
			if (!(gameMap->mapbuf[i].type >> 24)){
				gameMap->mapbuf[i].type ^= WAVE | FLOOR_BLOCK;
				//clear wave infomation
				gameMap->mapbuf[i].clearBWInfo();
				reflag = true;
			}	
			continue;
		}
	}
	return 0;
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
		int tmp = realTimeDealing();
		if (tmp < 0) {
//			goto bg;
			cls();
			puts("");
			exitRealTime();
			fclose(flog);
			SexitRealTime(); 
			showCursor();
			tmp==-1?
			printf("\033[%dm%s\033[0m wins.\nCongratulations!\n", players[aliveP].col, players[aliveP].Name):
			printf("\033[%dmNO BODY WINS.\033[0m\n", Red+Intense+Fore);
			printw("PRESS ESC TO EXIT...\n");
			while(1) if(GetKeyState(VK_ESCAPE) & 0x8000) break;
			return 0;
		}
//			_getch();
		if(GetKeyState(VK_ESCAPE) & 0x8000){
			cls();
			puts("");
			exitRealTime();
			fclose(flog);
			SexitRealTime(); 
//				fflush(stdin);
			showCursor();
			return 0;
		}
		if(GetKeyState('P') & 0x8000){
			Sleep(50);
			puts("\033[4;93mPausing - press R to resume.\033[0m");
			fflush(stdout);
			exitRealTime();
			while(1) if(GetKeyState('R') & 0x8000) break;
			enterRealTime();
			reflag |= 1;
		}
		for(int i = 0; i < playerCnt; i++){
			if (!players[i].keyCatch.dealInput(' ', &players[i])) {
				players[i].movCnt = players[i].spd;
				reflag |= 1;
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


