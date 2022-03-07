#include "maingame.h"

int main(int argc, char **argv){
	
	puts("Welcome into the Game PaoPaoTang(PPT)!");
	puts("Author: Litrehinn");
	puts("This is the command line for lanuching or debuging.");
	char command[25];
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
		if (strCommand == "help-debug") {
			puts("Not established yet.");
			continue;
		}
		if (strCommand == "map-edit"){
			mapEditer();
			continue;
		}
		puts("Sorry, we haven't such command, try using 'help'.");
	}
	return 0;
}
