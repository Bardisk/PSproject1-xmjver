#include "map.h"

int mapData::load(int fidx=0){
	char fName[10];
	strcpy(fName, RAWMNAME);
	fName[7] = fidx + '0';
	FILE *fp = fopen(fName, "rb");
	if (!fp) {
		printf("%s is not found.", fName);
		return -1;
	}
	fread(&szN, sizeof int, 1, fp);
	fread(&szM, sizeof int, 1, fp);
	fread(mapbuf, sizeof Node, szN*szM, fp);
	return 0;
}
int mapData::save(int fidx=0){
	char fName[10];
	strcpy(fName, RAWMNAME);
	fName[7] = fidx + '0';
	FILE *fp = fopen(fName, "wb");
	if (!fp) {
		printf("%s is not found.", fName);
		return -1;
	}
	fwrite(&szN, sizeof int, 1, fp);
	fwrite(&szM, sizeof int, 1, fp);
	fwrite(mapbuf, sizeof Node, szN*szM, fp);
	return 0;
}

int mapData::draw(){
	for (int i = 0; i < szN * szM; i++) {
		nodeInfo now = mapbuf[i].getInfo();
		switch (s1) {
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
		}
		if (s1 == 'F')
			switch (s2) {
				case 'S':
					putchar('S')
					break;
				case 'L':
					putchar('L');
					break;
			}
		if ((i+1) % szM == 0) puts("");
	}
}
