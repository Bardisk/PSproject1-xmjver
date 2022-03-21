
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <ctime>
#include <windows.h>
 
int main()
{
   	char buff[1024];
   	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   	DWORD mode;
   	GetConsoleMode(hConsole, &mode);
	SetConsoleMode(hConsole, mode | 0x0004);
   	memset( buff, '\0', sizeof( buff ));
   	fprintf(stdout, "∆Ù”√»´ª∫≥Â\n");
   	setvbuf(stdout, buff, _IOFBF, 1024);
	while(1){
		fwrite("\33[H\33[2J\33[3J", 1, 12, stdout);
		for(int i=1;i<=100;i++)
		fprintf(stdout, "%c",i%10?(rand()%26+'a'):'\n');
		fprintf(stdout, "\n");
		fflush( stdout );
		Sleep(25);
	}
   
 
   Sleep(500);
 
   return(0);
}

