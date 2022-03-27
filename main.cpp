#include "maingame.h"

HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_CURSOR_INFO cci;
COORD startUp={0,0};

char buff[4096];
bool isRealTimeMode = false;
int generateFactor = DEFAULT_FACTOR;
clock_t lastRespondTime, lastLoadedTime;
int tickCntSinceLoaded = 0;

mapData mainMap(0);
mainGame game;

int main(int argc, char **argv){
//	GetConsoleCursorInfo(hOutput, &cci);
//	setcol(ForeYellow); 
	puts("Welcome into the Game PaoPaoTang(PPT)!");
	puts("Author: Litrehinn");
	puts("This is the command line for lanuching or debuging.");
	char command[25];
	DWORD mode;
	GetConsoleMode(hOutput, &mode);
	SetConsoleMode(hOutput, mode|0x0004);
	//hideCursor();
//	printf("\033[?25l");
//	printf("%d\n",stdout);
//	SenterRealTime();
//	setvbuf(stdout, buff, _IOFBF, 4096);
//	printf("%d\n",stdout);
//	system("pause");
//	SexitRealTime();
//	fflush(stdout);
	while (~(printf(">> "), scanf("%s", command))){
		if (strlen(command) >= 15) {
			puts("Too long! Try using 'help' to get available commands.");
			continue;
		}
		
		std::string strCommand(command);
		if (strCommand == "help") {
			FILE* fp = fopen("etcs/help.txt","r");
			if (!fp) {
				puts("Help txt not found!");
				continue;
			}
			else {
				char buf[2001];
				while(fgets(buf, 2000, fp)) printf(buf);
				puts("");
			}
			fclose(fp);
			continue;
		}
		if (strCommand == "set-factor") {
			printf("Value? ");
			int v;
			scanf("%d", &v);
			generateFactor = v;
			continue;
		}
		if (strCommand == "help-debug") {
			putr("Warning:\nThese advanced options are intended for developers for the present.\nTheir usage may cause unexpected bugs or exceptions for the main game.");
			FILE* fp = fopen("etcs/helpadvanced.txt","r");
			if (!fp) {
				puts("Help txt not found!");
				continue;
			}
			else {
				char buf[2001];
				while(fgets(buf, 2000, fp)) printf(buf);
				puts("");
			}
			fclose(fp);
			continue;
		}
		if (strCommand == "reset") {
			mainMap.load(0);
			game = mainGame();
			continue;
		}
		if (strCommand == "quit") {
			return 0;
		}
		if (strCommand == "load-map") {
			printf("Index? ");
			int idx;
			scanf("%d", &idx);
			mainMap.load(idx);
			continue;
		}
		if (strCommand == "save-map") {
			printf("Index? ");
			int idx;
			scanf("%d", &idx);
			mainMap.save(idx);
			continue;
		}
		
		if (strCommand == "map-edit") {
			mapEditor editor(&mainMap);
			editor.main();
			continue;
		}
		
		if (strCommand == "load-into") {
			game.loadMap(&mainMap);
			continue;
		}
		
		if (strCommand == "load") {
			printf("Index? ");
			int idx;
			scanf("%d", &idx);
			game.load(idx);
			continue;
		}
		if (strCommand == "save") {
			printf("Index? ");
			int idx;
			scanf("%d", &idx);
			game.save(idx);
			continue;
		}
		if (strCommand == "game") {
			game.loadMap(&mainMap);
			game.main();
			puts("Welcome into the Game PaoPaoTang(PPT)!");
			puts("Author: Litrehinn");
			puts("This is the command line for lanuching or debuging.");
			continue;
		}
		puts("Sorry, we haven't such command, try using 'help'.");
	}
	
	CloseHandle(hOutput);
	return 0;
}
