#include "base.h"

#define B_AND_I_ERROR 0xfffb

nodeInfo::nodeInfo(const unsigned int &typ){
	type = typ & 65535;
	if (type & HAS_ITEM) {
		info.value = type >> 16;
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
			info.bvalue.level = (type >> 16) & 255;
			info.bvalue.lastTime = type >> 24;
		}
	}
}

nodeInfo node::getDesc(char *Desc){
	nodeInfo now(type);
	if(type & 0x8000) sprintf(Desc, "WARNING: the type is an error\n");
	sprintf(Desc, "************************\n");
	fsprintf(Desc, "NODE INFORMATION\n");
	switch (now.s1) {
		case 'H':
			fsprintf(Desc, "type: H(HARD_WALL)\n");
			break;
		case 'S':
			fsprintf(Desc, "type: S(SOFT_WALL)\n");
			break;
		case 'F':
			fsprintf(Desc, "type: F(FLOOR_BLOCK)\n");
			break;
		case 'B':
			fsprintf(Desc, "type: B(BOMB) with level:%d and last_time:%d\n", now.info.bvalue.level, now.info.bvalue.lastTime);
			break;
	}
	switch (now.s2) {
		case 'S':
			fsprintf(Desc, "item: S(SPEED_UP) with value:%d\n", now.info.value);
			break;
		case 'L':
			fsprintf(Desc, "item: L(LEVEL_UP) with value:%d\n", now.info.value);
			break;
	}
	fsprintf(Desc, "************************");
	return now;
}
