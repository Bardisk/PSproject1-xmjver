#ifndef _BASE_H_
#define _BASE_H_
#include <cstdio>
#include <windows.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include "color.h" 

#define MAXMSIZE 10000

//int a;
struct cursor{
	int x, y;
	int N, M;
	cursor(int xx=0, int yy=0, int nn=0, int mm=0) : x(xx), y(yy), N(nn), M(mm) {};
	inline int up(){return x?(x-=1):-1;}
	inline int le(){return y?(y-=1):-1;}
	inline int dw(){return x<N-1?(x+=1):-1;}
	inline int ri(){return y<M-1?(y+=1):-1;}
	inline int calNum(){return x*N+M;}
};

//this makes a template string for filename 
const char RAWMNAME[] = "maps/map .dat";
const char RAWSNAME[] = "savs/sav .dat";

//#define printw(xx) setcol(ForeYellow|ForeInt);printf(xx);rescol()
#define printw(...) setcol(ForeYellow|ForeInt),printf(__VA_ARGS__),rescol()
#define putw(xx) setcol(ForeYellow|ForeInt),puts(xx),rescol()
#define fsprintf(string, ...) sprintf(string+strlen(string), __VA_ARGS__)

//type accounts for lower 16 bits.
//for bomb higher 8 bits for last-time, lower 8 bits for level.
//for others higher 16 bits accounts for value.
#define FLOOR_BLOCK 0x00000001
#define HARD_WALL 0x00000002
#define SOFT_WALL 0x00000004
#define BOMB 0x00000008
#define HAS_ITEM 0x00000010
#define SPEED_UP 0x00000020
#define LEVEL_UP 0x00000040

struct nodeInfo{
	short type;
	char s1,s2;
	union{
		//used for bomb 
		struct{
			short lastTime;
			short level;
		}bvalue;
		int value;
	}info;
	nodeInfo(const unsigned int &typ);
};

struct drawSettings{
	
};
//8Byte

struct node{
	unsigned int type;
	node(const unsigned int &t=0) : type(t) {};
	inline nodeInfo getInfo(){return nodeInfo(type);};
	nodeInfo getDesc(char *Desc);
};
//4Byte

#endif
