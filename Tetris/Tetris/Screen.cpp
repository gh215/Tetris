#include "Tetris.h"

void Screen::showConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(out, &cursorInfo);
}

void Screen::drawBorders()
{
	for (int y = 0; y < SCREEN_HEIGHT; ++y)
	{
		if (y == 0 || y == SCREEN_HEIGHT - 1)
		{
			for (int x = 0; x < SCREEN_WIDTH; ++x)
			{
				drawSymb('-', x, y);
			}
		}
		else
		{
			drawSymb('|', 0, y);
			drawSymb('|', SCREEN_WIDTH - 1, y);
		}
	}
}

void Game::spawnFigure()
{
	vector<vector<vector<bool>>> shapes =
	{
		{{0,0,0,0,0},
		 {0,0,0,0,0},
		 {0,1,1,1,1},
		 {0,0,0,0,0},
		 {0,0,0,0,0}},
		{{0,0,0,0,0},
		 {0,0,0,0,0},
		 {0,1,1,1,0},
		 {0,0,1,0,0},
		 {0,0,0,0,0}},
	};

	int randomIndex = rand() % shapes.size();
	currentFigure = Figure(shapes[randomIndex], { SCREEN_WIDTH / 4 - 2, 0 });
}

