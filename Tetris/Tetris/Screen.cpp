#include "Tetris.h"

void Screen::showConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(out, &cursorInfo);
}

void Screen::drawRect(int x, int y, int width, int height, symbol border)
{
    if (x < 0 || y < 0 || x + width > SCREEN_WIDTH / 2 || y + height > SCREEN_HEIGHT)
    {
        return;
    }

    for (int i = 0; i < width; ++i)
    {
        putSymb(border, { x + i, y });
        putSymb(border, { x + i, y + height - 1 });
    }

    for (int j = 1; j < height - 1; ++j)
    {
        putSymb(border, { x, y + j });
        putSymb(border, { x + width - 1, y + j });
    }
}


