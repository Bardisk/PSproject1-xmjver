#ifndef _BASE_H_
#define _BASE_H_
#include <cstdio>
#include <windows.h>
#include <string>
#include<cstring>

#define MAXMSIZE 10000

struct pair{
	int x, y;
	pair(int xx=0, int yy=0) : x(xx), y(yy) {};
};
struct node{
	int type, valuex, valuey;
	node(int t=0, int vx=0, int vy=0) : type(t), valuex(vx), valuey(vy) {};
};



#endif
