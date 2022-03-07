#include<conio.h>
#include<cstdio>
#include<windows.h>
#include<ctime>
#include<cstdlib>
int tester(){
	puts("--TESTOR--\nPress any key...");
	while(1){
//		Sleep(900);
		if(_kbhit()){
			char ch=_getch();
			if(ch==0x1b){
				puts("The program is terminating...");
				Sleep(500);
				break;
			}
			system("cls");
			puts("--TESTOR--\nPress any key...");
			printf("u've pressed: %c \n",ch);
		}
	}
}
