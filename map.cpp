#include "map.h"

extern char buff[];
extern HANDLE hOutput;

extern bool isRealTimeMode;
extern clock_t lastRespondTime, lastLoadedTime;
extern int tickCntSinceLoaded; 
extern int generateFactor;

void cursor::renew(mapData *map) {N = map ->szN; M = map -> szM;}

int mapData::load(int fidx){
	char fName[20];
	strcpy(fName, RAWMNAME);
	fName[8] = fidx + '0';
	FILE *fp = fopen(fName, "rb");
	if (!fp) {
		printw("WARNING: %s is not found.\n", fName);
		return -1;
	}
	szN = szM = 0;
	memset(mapbuf, 0, sizeof mapbuf);
	fread(&szN, sizeof (int), 1, fp);
	fread(&szM, sizeof (int), 1, fp);
	fread(mapbuf, sizeof (node), szN*szM, fp);
	fclose(fp);
	return 0;
}

mapData::mapData(int fidx){
	if (load(fidx) == -1){
		putw("WARNING: Reading default map failed.");szN = szM = 0;
	}
}

int mapData::save(int fidx){
	char fName[20];
	strcpy(fName, RAWMNAME);
	fName[8] = fidx + '0';
	FILE *fp = fopen(fName, "wb");
	if (!fp) {
		printw("WARNING: %s is not found.\n", fName);
		return -1;
	}
	fwrite(&szN, sizeof (int), 1, fp);
	fwrite(&szM, sizeof (int), 1, fp);
	fwrite(mapbuf, sizeof (node), szN*szM, fp);
	fclose(fp);
	return 0;
}

int mapData::check(){
	for (int i = 0; i < szN * szM; i++) {
		if (!mapbuf[i].type) return -1;
	}
	return 0;
}

int mapData::draw(drawSettings dss){
	for (int i = 0; i < szN * szM; i++) {
		int tmp = dss.calHit(i);
		int bgcol = ((tmp & HIT_CURSOR)) ? Back + Intense : 0; 
		nodeInfo now = mapbuf[i].getInfo();
		setcol(bgcol);
		if (tmp & HIT_PLAYER) {
			setcol((tmp >> 16) & 255);
//			setcol(Back + Intense);
			setcol(4);
			putchar((char) ((tmp >> 8) & 255));
			rescol();
			putchar(' ');
			continue;
		}
		switch (now.s1) {
			case 'H':
				setcol(Fore+Yellow+Intense);
				putchar('#');
				break;
			case 'S':
				setcol(Fore+Yellow);
				putchar('=');
				break;
			case 'F':
				if (now.s2 == 'N')
					putchar(' ');
				else 
					setcol(Fore+Cyan+Intense);
				break;
			case 'B':
				setcol(Fore+Intense+Red);
				putchar('@');
				break;
			case 'W':
				setcol(Fore+Red);
				putchar('*');
				break;
			case 'U':
				putchar('U');
				break;
		}
		if (now.s1 == 'F')
			switch (now.s2) {
				case 'S':
					putchar('S');
					break;
				case 'L':
					putchar('L');
					break;
			}
		rescol();
		putchar(' ');
		if ((i+1) % szM == 0) puts("");
		
	}
	return 0;
}

int mapData::revs(){
	node tmpbuf[MAXMSIZE];
	std::swap(szN, szM);
	for (int i = 0; i < szN * szM; i++)
		tmpbuf[i] = mapbuf[i];
	for (int i = 0; i < szN; i++)
		for(int j = 0; j < szM; j++)
			mapbuf[i * szM + j] = tmpbuf[j * szN + i];
	return 0;
}

int mapEditor::refresh(bool &bsflag){
	char tmpPrint[500];
//	resetCursor();
	cls();
	printf("Avg Tick: %8.4lf ms\n", (clock() - lastLoadedTime)/(double) tickCntSinceLoaded);
	printf("Now cursor: (%2d, %2d)\n", cur.x, cur.y);
	map->mapbuf[cur.calNum()].getDesc(tmpPrint);
	puts(tmpPrint);
	
	drawSettings dss;
	if (moveCounter <= 1*CURSOR_SPEED/20)
		dss.addHighlight(&cur);
	if (moveCounter > (5*CURSOR_SPEED+19)/20)
		dss.addHighlight(&lst);
	map->draw(dss);
	fflush(stdout);
	//sweep rubbish
	return 0;
}

int mapEditor::edit(){
	FILE *flog = fopen("realtimelog.txt", "w");
	fprintf(flog, "%lld\n", time(0));
	cls();
	
	SenterRealTime();
	hideCursor();
	enterRealTime();
	bool reflag = false;
	bool bsflag = false;
	refresh(bsflag);
	while (true) {
		clock_t nowt = clock();
		if (nowt - lastRespondTime < GAMETICK)
			continue;
		++tickCntSinceLoaded;
		fprintf(flog, "tick: %d, timegap: %ld\n", tickCntSinceLoaded, nowt - lastRespondTime);
		lastRespondTime = nowt;
		if (moveCounter) {
			bool flagtmp = (moveCounter <= 1*CURSOR_SPEED/20 || moveCounter > (5*CURSOR_SPEED+19)/20);
			--moveCounter;
			flagtmp ^= (moveCounter <= 1*CURSOR_SPEED/20 || moveCounter > (5*CURSOR_SPEED+19)/20);
			reflag |= flagtmp;
		}
			
		if (_kbhit()){
			char ch = _getch();
			switch (ch) {
				case 0x1b: //esc
					cls();
					puts("Exit from edit mode.");
					exitRealTime();
					fclose(flog);
					showCursor();
					SexitRealTime(); 
					return 0;
				case 'I':
					ineditMode ^= 1;
					reflag |= 1;
					break;
				case '\r':
					showCursor();
					exitRealTime();
					fflush(stdout),setvbuf(stdout,buff,_IONBF,4096);
					if (!~map->mapbuf[cur.calNum()].changeNode())
						Sleep(500);
					hideCursor();
					setvbuf(stdout,buff,_IOFBF,4096);
					enterRealTime(); 
					reflag |= 1;
					bsflag |= 1;
					break;
				default:
					reflag |= ~editInput(ch);
			}
		}
		reflag ? (reflag=refresh(bsflag)) : 0;
	}
	fclose(flog);
	exitRealTime();
	showCursor();
	SexitRealTime(); 
	
	return 0;
}

int mapEditor::editInput(char x, int conflag){
	//reject to respond in some ticks
//	static int cnt = 0;
	if (moveCounter) return -1;
	moveCounter = CURSOR_SPEED - 1;
	lst = cur;
	switch(x){
		case 'w':
			return cur.up();
		case 'W':
			return cur.up();
		case 'd':
			return cur.ri();
		case 'D':
			return cur.ri();
		case 's':
			return cur.dw();
		case 'S':
			return cur.dw();
		case 'a':
			return cur.le();
		case 'A':
			return cur.le();
		default:
			return -1;
	}
	return 0;
}

int mapEditor::main(){
	cls();
	puts("This is the command line of mapEditor.");
	char command[25];
	while (~(printf("mapEdit>> "), scanf("%s", command))){
		if (strlen(command) >= 15) {
			puts("Too long.");
			continue;
		}
		std::string strCommand(command);
		if (strCommand == "edit") {
			edit();
			continue;
		}
		if (strCommand == "setSz") {
			printf("N = ");
			int N;
			scanf("%d", &N);
			printf("M = ");
			int M;
			scanf("%d", &M);
			map->szN = cur.N = N;
			map->szM = cur.M = M;
			continue;
		}
		if (strCommand == "load") {
			printf("Index? ");
			int idx;
			scanf("%d", &idx);
			map->load(idx);
			cur.renew(map);
			continue;
		}
		if (strCommand == "save") {
			printf("Index? ");
			int idx;
			scanf("%d", &idx);
			map->save(idx);
			continue;
		}
		if (strCommand == "revs") {
			map->revs();
			std::swap(cur.x, cur.y);
			std::swap(cur.N, cur.M);
			continue;
		} 
		puts("Invaild Command");
	}
	cls();
	puts("Exiting from mapEditor...");
	return 0;
}

//replace a node by WAVE
int mapData::repWave(const cursor &tmpCur, const int waveTemp, player *owner){
	if (mapbuf[tmpCur.calNum()].type & HARD_WALL) return -1;
	//if floor, clear the item and set the type to wave
	if (mapbuf[tmpCur.calNum()].type & FLOOR_BLOCK) {
		mapbuf[tmpCur.calNum()].type = waveTemp;
		return 0;
	}
	if (mapbuf[tmpCur.calNum()].type & WAVE) {
		mapbuf[tmpCur.calNum()].type = waveTemp;
		return 0;
	}
	if (mapbuf[tmpCur.calNum()].type & BOMB) return 0;
	//if soft wall, expose the item or generate an item randomly
	if (mapbuf[tmpCur.calNum()].type & SOFT_WALL) {
		mapbuf[tmpCur.calNum()].type ^= SOFT_WALL;
		mapbuf[tmpCur.calNum()].type |= waveTemp;
		if (!(mapbuf[tmpCur.calNum()].type & HAS_ITEM)) {
			owner->sco += WALL_SCORE;
			if(generateFactor <= 0) return -1;
			int rnd = rand() % generateFactor;
			if (!rnd) {
				mapbuf[tmpCur.calNum()].type |= HAS_ITEM;
				int rnd2 = rand() % 3;
				if(!rnd2){
					mapbuf[tmpCur.calNum()].type |= SPEED_UP;
					mapbuf[tmpCur.calNum()].type |= (2 << 16);
				}
				else{
					mapbuf[tmpCur.calNum()].type |= LEVEL_UP;
					mapbuf[tmpCur.calNum()].type |= (1 << 16);
				}
			}
		}
		return -1;
	}
	return 1;
}

//trigger a bomb in the map
//return the id of the owner
int mapData::triggerBomb(const cursor &cur, player* owner){
	nodeInfo tmp = mapbuf[cur.calNum()].getInfo();
	cursor tmpCur = cur;
	int waveTemplate = WAVE | (WAVE_TIME << 24) | (tmp.info.bvalue.owner << 20);
	mapbuf[cur.calNum()].type = waveTemplate;
	for (int i = 0; i < tmp.info.bvalue.level; i++) {
		if (!~tmpCur.le()) break;
		if (!~repWave(tmpCur, waveTemplate, owner)) break;
	}
	tmpCur = cur;
	for (int i = 0; i < tmp.info.bvalue.level; i++) {
		if (!~tmpCur.ri()) break;
		if (!~repWave(tmpCur, waveTemplate, owner)) break;
	}
	tmpCur = cur;
	for (int i = 0; i < tmp.info.bvalue.level; i++) {
		if (!~tmpCur.up()) break;
		if (!~repWave(tmpCur, waveTemplate, owner)) break;
	}
	tmpCur = cur;
	for (int i = 0; i < tmp.info.bvalue.level; i++) {
		if (!~tmpCur.dw()) break;
		if (!~repWave(tmpCur, waveTemplate, owner)) break;
	}
	owner->bmbOwning += 1;
	return tmp.info.bvalue.owner;
}
