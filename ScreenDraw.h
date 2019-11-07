#pragma once
#include <string>

#define BLACK         	0
#define RED           	9
#define GREEN         	34
#define YELLOW 			178
#define BLUE 			25
#define PURPLE 			55
#define DARKGREEN 		28
#define WHITE 			15
#define LIGHTGREEN      47
#define BROWN           94
#define ORANGE          208
#define GRAY            239
#define LIGHTGRAY       247
#define LIGHTYELLOW     11
#define LIGHTBLUE       27
#define SQUASHGREEN     76
#define PINK            165
#define DARKRED         88

using namespace std;

void drawPixel(int x, int y, int color);
void drawPixelC(int x, int y, char c1, char c2, int color, int backColor);
void screenClear();
void clearRect(int x, int y, int w, int h);
void drawLine(int x, int y, int l, bool hori, int color);
void drawLineC(int x, int y, int l, char c1, char c2, bool hori, int color, int backColor);
void drawRect(int x, int y, int w, int h, int color);
void drawWholeRect(int x, int y, int w, int h, int color);
void drawText(int x, int y, string str, int color, int backColor);
