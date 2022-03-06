#ifndef _BASE_H
#define _BASE_H
#include <cstdio>
#include <windows.h>
#include <string>

#define MAXMSIZE 10000

struct pair{
	int x, y;
	pair(int xx=0, int yy=0) : x(xx), y(yy) {};
}
struct node{
	int type, valuex, valuey;
	node(int t=0, int vx=0, int vy=0) : type(t), valuex(vx), valuey(vy) {};
}

class mapData{
	//data members
	int szN, szM;
	node mapbuf[MAXMSIZE];
	
	//public methods
	public:
		mapData() : szN(0), szM(M) {};
		mapData(int N, int M) : szN(N), szM(M) {};
		int load(int fidx=0);
		mapData(int fidx);
}

struct player{
	pair pos;
	int spd, hp, lvl;
	std::string Name;
}

class mainGame{
	int playerCnt;
	player players[5];
	int load(int fidx=0);
	int save(int fidx=0);
}

#endif