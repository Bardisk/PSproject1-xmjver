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
//8Byte

struct node{
	unsigned int type;
	node(const unsigned int &t=0) : type(t) {};
	inline nodeInfo getInfo(){return nodeInfo(type);};
	nodeInfo getDesc(char *Desc);
};
//4Byte

#endif
