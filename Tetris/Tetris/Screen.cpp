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
    if (x < 0 || y < 0 || x + width > logicalWidth() || y + height > logicalHeight())
    {
        return;
    }

    for (int row = y; row < y + height; ++row)
    {
        if (row == y || row == y + height - 1)
        {
            for (int col = x; col < x + width; ++col)
            {
                drawSymb(border.first, logicalToPhysicalX(col), row);
            }
        }
        else
        {
            drawSymb(border.second, logicalToPhysicalX(x), row);
            drawSymb(border.second, logicalToPhysicalX(x + width - 1), row);
        }
    }
}


