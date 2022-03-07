#ifndef _COLOR_H_
#define _COLOR_H_

//#define setcol(hOP,a) SetConsoleTextAttribute(hOP,a)

#define ForeBlue FOREGROUND_BLUE
#define ForeGreen FOREGROUND_GREEN
#define ForeRed FOREGROUND_RED
#define ForeInt FOREGROUND_INTENSITY
#define ForeYellow FOREGROUND_GREEN|FOREGROUND_RED
#define ForeWhite FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_BLUE
#define BackBlue BACKGROUND_BLUE
#define BackGreen BACKGROUND_GREEN
#define BackRed BACKGROUND_RED
#define BackInt BACKGROUND_INTENSITY
#define setcol(col) SetConsoleTextAttribute(hOutput,col)
#define rescol() SetConsoleTextAttribute(hOutput,ForeWhite)

#endif
