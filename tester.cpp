#include<conio.h>
#include<cstdio>
#include<windows.h>
#include<ctime>
#include<cstdlib>
int main(){
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hOutput, &mode);
	SetConsoleMode(hOutput, mode|0x0004);
	printf("\033[%dm\n",94);
}
