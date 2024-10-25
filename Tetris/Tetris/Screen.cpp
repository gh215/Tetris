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
	// Проходим по всей высоте экрана
	for (int y = 0; y < SCREEN_HEIGHT; ++y)
	{
		// Если это верхняя или нижняя строка
		if (y == 0 || y == SCREEN_HEIGHT - 1)
		{
		
			drawSymb(' ', 0, y);  // Рисуем пробел в начале строки
			for (int x = 1; x < SCREEN_WIDTH - 1; ++x)
			{
				drawSymb('-', x, y);
			}
			drawSymb(' ', SCREEN_WIDTH - 1, y);  // Рисуем пробел в конце строки
		}
		else
		{
			// Для остальных строк рисуем вертикальные границы
			drawSymb('|', 0, y);                    // Левая граница
			drawSymb(' ', 1, y);                    // Отступ после левой границы
			drawSymb(' ', SCREEN_WIDTH - 2, y);     // Отступ перед правой границей
			drawSymb('|', SCREEN_WIDTH - 1, y);     // Правая граница
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
		{{0,0,0,0,0},
		 {0,0,1,1,0},
		 {0,0,1,1,0},
		 {0,0,0,0,0},
		 {0,0,0,0,0}},
		{{0,0,0,0,0},
		 {0,0,1,0,0},
		 {0,0,1,0,0},
		 {0,0,1,1,0},
		 {0,0,0,0,0}},
		{{0,0,0,0,0},
		 {0,0,1,0,0},
		 {0,0,1,0,0},
		 {0,1,1,0,0},
		 {0,0,0,0,0}},
		{{0,0,0,0,0},
		 {0,0,0,0,0},
		 {0,0,1,1,0},
		 {0,1,1,0,0},
		 {0,0,0,0,0}},
	    {{0,0,0,0,0},
		 {0,0,0,0,0},
		 {0,1,1,0,0},
		 {0,0,1,1,0},
		 {0,0,0,0,0}}
	};

	int randomIndex = rand() % shapes.size();
	currentFigure = Figure(shapes[randomIndex], { SCREEN_WIDTH / 4 - 2, 0 });
}

