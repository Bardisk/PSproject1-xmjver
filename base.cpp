#include "base.h"
#include "ai.h"
#include "map.h" 

#define B_AND_I_ERROR 0xfffb

extern HANDLE hOutput;

void setcol(int col){
	char colCom[50];
	sprintf(colCom,"\033[%dm",col);
	printf(colCom);
	return ;
}

int keyCatcher::dealInput(int input, player *target){
	if(target->isAI) return dealAI(target);
	//arrow keys can be only owned by one!
//	bool flag=false;
	if (GetKeyState(setter) & 0x8000)
		return target->setBomb(), 1;
	if (target->movCnt) return -1;
	if (GetKeyState(up) & 0x8000)
		return target->up();
	if (GetKeyState(dn) & 0x8000)
		return target->dw();
	if (GetKeyState(ri) & 0x8000)
		return target->ri();
	if (GetKeyState(le) & 0x8000)
		return target->le();
	return -1;
}

int keyCatcher::dealAI(player *target){
	int tmpDec = target->robot->decision(target);
	if (tmpDec == DECIDE_SETBOMB)
		return target->robot->success(target->setBomb()), 1;
	if (!tmpDec || target->movCnt) return -1;
	switch (tmpDec) {
		case DECIDE_UP:
			return target->robot->success(target->up());
		case DECIDE_DOWN:
			return target->robot->success(target->dw());
		case DECIDE_LEFT:
			return target->robot->success(target->le());
		case DECIDE_RIGHT:
			return target->robot->success(target->ri());
		default:
			return -1;
	}
	return -1;
}

nodeInfo::nodeInfo(const unsigned int &typ){
	type = typ & 65535;
	if (type & HAS_ITEM) {
		info.value = typ >> 16;
		if (type & SPEED_UP) s2 = 'S';
		if (type & LEVEL_UP) s2 = 'L';
	}
	else s2 = 'N';
	
	if (!type)
		s1 = 'U';
	if (type & SOFT_WALL)
		s1 = 'S';
	if (type & HARD_WALL)
		s1 = 'H';
	if (type & FLOOR_BLOCK)
		s1 = 'F';
	if (type & BOMB) {
		s1 = 'B';
		if (s2 != 'N')
			type = B_AND_I_ERROR;
		else {
			info.bvalue.level = (typ >> 16) & 255;
			info.bvalue.lastTime = typ >> 24;
		}
	}
	if (type & WAVE) {
		s1 = 'W';
//		if (s2 != 'N')
//			type = B_AND_I_ERROR;
//		else {
			info.bvalue.level = (typ >> 16) & 255;
			info.bvalue.lastTime = typ >> 24;
//		}
	}
}

int node::changeNode(){
	char s1, s2;
	type = 0;
	printf("Typ? ");
	scanf(" %c", &s1);
	switch (s1) {
		case 'B': {
			int lt, lvl;
			printf("\tLast Time? ");
			scanf("%d", &lt);
			printf("\tLevel?");
			scanf("%d", &lvl);
			type = (((lt << 8) | lvl) << 16) | BOMB; 
			break;
		}
		case 'W': 
			type = WAVE;
			break;
		case 'S':
			type = SOFT_WALL;
			break;
		case 'H':
			type = HARD_WALL;
			break;
		case 'F':
			type = FLOOR_BLOCK;
			break;
		default:
			putr("Invaild type.");
			return -1;
	}
	printf("Itm? ");
	scanf(" %c", &s2);
	switch (s2) {
		case 'N':
			break;
		case 'S': {
			int v;
			printf("Value? ");
			scanf("%d", &v);
			type |= (v << 16) | SPEED_UP | HAS_ITEM;
			break;
		}
		case 'L' : {
			int v;
			printf("Value? ");
			scanf("%d", &v);
			type |= (v << 16) | LEVEL_UP | HAS_ITEM;
			break;
		}
		default:
			putr("Invaild item.");
			return -1;
	}
	return 0;
}

nodeInfo node::getDesc(char *Desc){
	nodeInfo now(type);
	if(type & 0x8000) sprintf(Desc, "WARNING: the type is an error\n");
	sprintf(Desc, "************************\n");
	fsprintf(Desc, "NODE INFORMATION\n");
	switch (now.s1) {
		case 'H':
			fsprintf(Desc, "type: H(HARD_WALL)                          \n");
			break;
		case 'S':
			fsprintf(Desc, "type: S(SOFT_WALL)                          \n");
			break;
		case 'F':
			fsprintf(Desc, "type: F(FLOOR_BLOCK)                        \n");
			break;
		case 'B':
			fsprintf(Desc, "type: B(BOMB) with level:%-2d and last_time:%-2d\n", now.info.bvalue.level, now.info.bvalue.lastTime);
			break;
		case 'W':
			fsprintf(Desc, "type: W(WAVE) with level:%-2d and last_time:%-2d\n", now.info.bvalue.level, now.info.bvalue.lastTime);
			break;
		default:
			fsprintf(Desc, "type: U(UNDEFINED)                          \n");
			break;
	}
	switch (now.s2) {
		case 'S':
			fsprintf(Desc, "item: S(SPEED_UP) with value:%-5d\n", now.info.value);
			break;
		case 'L':
			fsprintf(Desc, "item: L(LEVEL_UP) with value:%-5d\n", now.info.value);
			break;
		default:
			fsprintf(Desc, "item: inexisting or undefined     \n");
	}
	fsprintf(Desc, "************************");
	return now;
}

char* player::getDesc(char *Desc, int idx){
	sprintf(Desc, "\033[%d;4m%s\033[0m: hp:%d level:%d speed:%d score:%d\n", col, Name, hp, lvl, spd, sco);
	return Desc;
}

cursor* drawSettings::addHighlight(cursor *now){
	return highlightsCnt > 9 ? NULL : (highlights[highlightsCnt++] = now);
}
player* drawSettings::addPlayer(player *now){
	return playersCnt > 9 ? NULL : (players[playersCnt++] = now);
}
int drawSettings::calHit(int pos){
	int ans = 0;
	for (int i = 0; i < playersCnt; i++) {
		if (players[i]->pos.calNum() == pos)
			ans |= HIT_PLAYER | (players[i]->Rpcr << 8) | (players[i]->col << 16);
	}
	for (int i = 0; i < highlightsCnt; i++){
		if (highlights[i]->calNum() == pos)
			ans |= HIT_CURSOR;
	}
	return ans;
}

bool player::isBanMove(char ch){
	for (int i = 0; i < strlen(banMove); i++)
		if (banMove[i] == ch) return true;
	return false;
}

int player::eatItem(){
	nodeInfo tmp = myMap->mapbuf[pos.calNum()].getInfo();
	if (tmp.s2 == 'S')
		spd -= tmp.info.value;
	if (tmp.s2 == 'L')
		lvl += tmp.info.value;
	//erase the item
	myMap->mapbuf[pos.calNum()].type ^= HAS_ITEM;
	myMap->mapbuf[pos.calNum()].type &= 65535;
	return 0;
}
int player::up(){
	if (pos.up() == -1) return -1;
	nodeInfo tmp = myMap->mapbuf[pos.calNum()].getInfo();
	if (isBanMove(tmp.s1)) {
		pos.dw();
		return -1;
	}
	if (tmp.s2 != 'N') eatItem();
	return 0;
}
int player::le(){
	if (pos.le() == -1) return -1;
	nodeInfo tmp = myMap->mapbuf[pos.calNum()].getInfo();
	if (isBanMove(tmp.s1)) {
		pos.ri();
		return -1;
	}
	if (tmp.s2 != 'N') eatItem();
	return 0;
}
int player::dw(){
	if (pos.dw() == -1) return -1;
	nodeInfo tmp = myMap->mapbuf[pos.calNum()].getInfo();
	if (isBanMove(tmp.s1)) {
		pos.up();
		return -1;
	}
	if (tmp.s2 != 'N') eatItem();
	return 0;
}
int player::ri(){
	if (pos.ri() == -1) return -1;
	nodeInfo tmp = myMap->mapbuf[pos.calNum()].getInfo();
	if (isBanMove(tmp.s1)) {
		pos.le();
		return -1;
	}
	if (tmp.s2 != 'N') eatItem();
	return 0;
}
int player::setBomb(){
	myMap->mapbuf[pos.calNum()].type = 0;
	myMap->mapbuf[pos.calNum()].type |= BOMB;
	myMap->mapbuf[pos.calNum()].type |= (lvl << 16);
	myMap->mapbuf[pos.calNum()].type |= (BOMB_TIME << 24);
	return 0;
}
