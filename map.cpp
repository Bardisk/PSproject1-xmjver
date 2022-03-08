#include "map.h"

extern HANDLE hOutput;

int mapData::load(int fidx){
	char fName[20];
	strcpy(fName, RAWMNAME);
	fName[8] = fidx + '0';
	FILE *fp = fopen(fName, "rb");
	if (!fp) {
		printw("WARNING: %s is not found.\n", fName);
		return -1;
	}
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

int mapData::draw(){
	for (int i = 0; i < szN * szM; i++) {
		nodeInfo now = mapbuf[i].getInfo();
		switch (now.s1) {
			case 'H':
				putchar('#');
				break;
			case 'S':
				putchar('=');
				break;
			case 'F':
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
	}
}

int mapEditor

int mapEditor::edit(){
	char command[25];
	char tmpPrint[500];
	map->mapbuf[cur.calNum()].getDesc(tmpPrint);
	puts(tmpPrint);
	map->draw();
	while (true){
		
	}
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
	}
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

