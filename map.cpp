#include "map.h"

extern HANDLE hOutput;
extern CONSOLE_CURSOR_INFO cci;
extern COORD startUp;

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

int mapData::draw(drawSettings dss){
	for (int i = 0; i < szN * szM; i++) {
		int tmp = dss.calHit(i);
		int bgcol = (tmp & HIT_CURSOR) ? BackInt : 0; 
		nodeInfo now = mapbuf[i].getInfo();
		setcol(0x7 | bgcol);
		if (tmp & HIT_PLAYER) {
			putchar(tmp >> 24);
			rescol();
			continue;
		}
		switch (now.s1) {
			case 'H':
				putchar('#');
				break;
			case 'S':
				putchar('=');
				break;
			case 'F':
				if (now.s2 == 'N')
					putchar(' ');
				break;
			case 'B':
				putchar('B');
				break;
			case 'U':
				putchar('U');
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
		if ((i+1) % szM == 0) puts("");
		rescol();
	}
	return 0;
}

int mapEditor::refresh(bool &bsflag){
	char tmpPrint[500];
	resetCursor();
	puts(ineditMode ? "EDITING" : "VIEWING");
	printf("Now cursor: (%d, %d)\n", cur.x, cur.y);
	map->mapbuf[cur.calNum()].getDesc(tmpPrint);
	puts(tmpPrint);
	drawSettings dss;
	dss.addHighlight(&cur);
	map->draw(dss);
	//sweep rubbish
	if (bsflag) {
		for(int i = 1; i < 10; i++)
			puts("                    ");
		bsflag = false;
		refresh(bsflag);
	}
	return 0;
}

int mapEditor::edit(){
	system("cls");
	hideCursor();
	bool reflag = false;
	bool bsflag = false;
	refresh(bsflag);
	while (true) {
		if (_kbhit()){
			char ch = _getch();
			switch (ch) {
				case 0x1b: //esc
					system("cls");
					puts("Exit from edit mode.");
					showCursor();
					return 0;
				case 'I':
					ineditMode ^= 1;
					reflag |= 1;
					break;
				case '\r':
					showCursor();
					if (!~map->mapbuf[cur.calNum()].changeNode())
						Sleep(500);
					hideCursor();
					reflag |= 1;
					bsflag |= 1;
					break;
				default:
					reflag |= ~editInput(ch);
			}
		}
		reflag ? (reflag=refresh(bsflag)) : 0;
	}
	showCursor();
	return 0;
}

int mapEditor::editInput(char x, int conflag){
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
	system("cls");
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
		if (strCommand == "load"){
			printf("Index? ");
			int idx;
			scanf("%d", &idx);
			map->load(idx);
			continue;
		}
		if (strCommand == "save"){
			printf("Index? ");
			int idx;
			scanf("%d", &idx);
			map->save(idx);
			continue;
		}
		puts("Invaild Command");
	}
	system("cls");
	puts("Exiting from mapEditor...");
	return 0;
}

