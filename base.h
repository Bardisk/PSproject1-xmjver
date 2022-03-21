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
#define GAMETICK 25
#define CURSOR_SPEED 10

//int a;
class mapData;
struct cursor{
	int x, y;
	int N, M;
	cursor(int xx=0, int yy=0, int nn=0, int mm=0) : x(xx), y(yy), N(nn), M(mm) {};
	inline int up(){return x?(x-=1):-1;}
	inline int le(){return y?(y-=1):-1;}
	inline int dw(){return x<N-1?(x+=1):-1;}
	inline int ri(){return y<M-1?(y+=1):-1;}
	void renew(mapData *map);
	inline int calNum(){return x*M+y;}
};

struct keycatcher{
	
};

struct player{
	cursor pos;
	int spd, hp, lvl, sco;
	char Name[15], Rpcr;
	int col; 
	player(const char *PName = NULL, char PRpcr = '$', int col = 0, cursor Ppos = cursor()): Rpcr(PRpcr){
		if (!PName) sprintf(Name, "Default Name");
		else strcpy(Name, PName);
		pos = Ppos;
		spd = 10;
		hp = 3;
		lvl = 1;
		sco = 0;
		this->col = col;
	}
	char* getDesc(char *Desc, int idx=0);
	char getHit(char s);
};

//this makes a template string for filename 
const char RAWMNAME[] = "maps/map .dat";
const char RAWSNAME[] = "savs/sav .dat";

//#define printw(xx) setcol(ForeYellow|ForeInt);printf(xx);rescol()
#define printw(...) setcol(Yellow+Intense+Fore),printf(__VA_ARGS__),rescol()
#define putw(xx) setcol(Yellow+Intense+Fore),puts(xx),rescol()
#define putr(xx) setcol(Red+Fore+Intense),puts(xx),rescol()
#define fsprintf(string, ...) sprintf(string+strlen(string), __VA_ARGS__)
#define resetCursor() SetConsoleCursorPosition(hOutput, startUp);
//#define hideCursor() GetConsoleCursorInfo(hOutput,&cci), cci.bVisible=false, SetConsoleCursorInfo(hOutput,&cci)
//#define showCursor() GetConsoleCursorInfo(hOutput,&cci), cci.bVisible=true, SetConsoleCursorInfo(hOutput,&cci)
#define hideCursor() printf("\033[?25l")
#define showCursor() printf("\033[?25h")
#define exitRealTime() isRealTimeMode = false
#define enterRealTime() lastLoadedTime = lastRespondTime = clock(), tickCntSinceLoaded = 1, isRealTimeMode = true
#define cls() printf("\033[H\033[2J\033[3J")
#define SenterRealTime() printf("\033[?1049h"),cls(),setvbuf(stdout,buff,_IOFBF,4096)
#define SexitRealTime() printf("\033[?1049l"),fflush(stdout),setvbuf(stdout,buff,_IONBF,4096);

//type accounts for lower 16 bits.
//for bomb higher 8 bits for last-time, lower 8 bits for level.
//for others higher 16 bits accounts for value.
#define FLOOR_BLOCK 0x0001
#define HARD_WALL 0x0002
#define SOFT_WALL 0x0004
#define BOMB 0x0008
#define HAS_ITEM 0x0010
#define SPEED_UP 0x0020
#define LEVEL_UP 0x0040
#define WAVE 0x0080

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

#define HIT_PLAYER 0x01
#define HIT_CURSOR 0x02
struct drawSettings{
	int playersCnt, highlightsCnt;
	player *players[10];
	cursor *highlights[10];
	public:
		drawSettings(): playersCnt(0), highlightsCnt(0) {};
		player* addPlayer(player *now);
		cursor* addHighlight(cursor *now);
		int calHit(int pos);
};

struct node{
	unsigned int type;
	node(const unsigned int &t=0) : type(t) {};
	inline nodeInfo getInfo(){return nodeInfo(type);};
	nodeInfo getDesc(char *Desc);
	int changeNode();
};
//4Byte

#endif
