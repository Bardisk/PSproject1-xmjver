#ifndef _MAP_H_
#define _MAP_H_
#include "base.h"
class mapData{
	//data members
	int szN, szM;
	node mapbuf[MAXMSIZE];
	
	//public methods
	public:
		mapData() : szN(0), szM(0) {};
		mapData(int N, int M) : szN(N), szM(M) {};
		int load(int fidx=0);
		int save(int fidx=0);
		mapData(int fidx);
};

void mapEditer();

#endif
