#include "ai.h"
#include "map.h"

int AI::decision(player *me)
{
	player tmpMe = *me;
	if (me->hp <= 0) return 0;
	if (step == 0) {
		if (~tmpMe.le())
			return lastDecision = DECIDE_LEFT;
		if (~tmpMe.ri())
			return lastDecision = DECIDE_RIGHT;
		if (~tmpMe.dw())
			return lastDecision = DECIDE_DOWN;
		if (~tmpMe.up())
			return lastDecision = DECIDE_UP;
		return 0;
	}
	if (step == 1) {
		return DECIDE_SETBOMB;
	}
	if (step == 2) {
		return lastDecision = 5 - lastMove;
	}
	if (step == 3) {
		return lastDecision = rand() % 5;
	}
	if (step == 4) {
		if (--frightenCnt == 0)
			return lastDecision = 5 - lastMove;
		return 0;
	}
	return 0;
}
