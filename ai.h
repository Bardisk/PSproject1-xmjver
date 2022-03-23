#ifndef _AI_H_
#define _AI_H_
#include "base.h"
#include "map.h"
#define DECIDE_LEFT 1
#define DECIDE_RIGHT 4
#define DECIDE_UP 2
#define DECIDE_DOWN 3
#define DECIDE_SETBOMB 5
struct player;
class AI{
	int step;
	int lastDecision;
	int frightenCnt;
	int lastMove;
	
	public:
		AI(){
			step = 0;
			frightenCnt = 0;
			lastDecision = 0;
			lastMove = 0;
		}
		int decision(player *me);
		int success(int rv){
			if (!~rv) return rv;
			if (step == 3) frightenCnt = 200;	
			step = (step + 1) % 5;
			lastMove = lastDecision;
			return rv;
		}
};
#endif
