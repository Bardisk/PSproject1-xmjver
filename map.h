#ifndef _MAP_H_
#define _MAP_H_
#include "base.h"
class mapData{
	//data members
	public:
		int szN, szM;
		node mapbuf[MAXMSIZE];
	
		//public methods
	
		mapData() : szN(0), szM(0) {};
		mapData(int N, int M) : szN(N), szM(M) {};
		int load(int fidx=0);
		int save(int fidx=0);
		int draw(drawSettings dss=drawSettings());
		int revs();
		int check();
		mapData(int fidx);
};

class mapEditor{
	mapData *map;
	bool ineditMode;
	int refresh(bool &bsflag);
	int moveCounter;
	public:
		cursor cur;
		cursor lst;
		mapEditor(mapData *targetMap): map(targetMap), ineditMode(false), moveCounter(0), cur(0, 0, targetMap->szN, targetMap->szM), lst(cur) {};
		int editInput(char x, int conflag=0);
		int edit();
		int main();
};

#endif
