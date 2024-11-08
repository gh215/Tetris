#include "Tetris.h"

void Heap::placeFigure(Figure& figure)
{
	vector<vector<bool>> shape = figure.getShape();
	Point pos = figure.getPosition();

	for (int row = 0; row < shape.size(); row++)
	{
		for (int col = 0; col < shape[row].size(); col++)
		{
			if (shape[row][col])
			{
				int x = pos.x + col;
				int y = pos.y + row;
				// Проверяем границы перед размещением
				if (y >= 0 && y < screen.logicalHeight() - 1 && x >= 0 && x < (screen.logicalWidth() - 1))
				{
					placedFigures[y][x] = true;
				}
			}
		}
	}
}

bool Heap::checkCollision(vector<vector<bool>> shape, Point position)
{
	// Проверяем каждую ячейку фигуры
	for (int row = 0; row < shape.size(); row++)
	{
		for (int col = 0; col < shape[row].size(); col++)
		{
			// Если в данной позиции есть часть фигуры
			if (shape[row][col])
			{
				int newX = position.x + col;
				int newY = position.y + row;

				// Проверяем выход за границы
				if (newX < 0 
					|| newX >= screen.logicalWidth() 
					|| newY >= screen.logicalHeight()
					|| (newY >= 0 && placedFigures[newY][newX]))
				{
					return true;
				}
			}
		}
	}
	return false; 
}

// Проверка и удаление заполненных линий
void Heap::checkLines()
{
	// Проверяем каждую строку снизу вверх
	for (int row = screen.logicalHeight() - 1; row >= 0; row--)
	{
		bool isLineFull = true;
		// Проверяем заполненность строки
		for (int col = 1; col < screen.logicalWidth() - 1; col++)
		{
			if (!placedFigures[row][col])
			{
				isLineFull = false;
				break;
			}
		}
		// Если строка заполнена
		if (isLineFull)
		{
			// Сдвигаем все строки выше текущей вниз
			for (int y = row; y > 0; y--)
			{
				for (int x = 0; x < screen.logicalWidth(); x++)
				{
					placedFigures[y][x] = placedFigures[y - 1][x];
				}
			}
			// Очищаем верхнюю строку
			for (int x = 1; x < screen.logicalWidth() - 1; x++)
			{
				placedFigures[0][x] = false;
			}
		}
		else
		{
			// Переходим к следующей строке только если текущая не была удалена
			row--;
		}
	}
}

// Отрисовка всех блоков
void Heap::draw()
{
	for (int y = 0; y < screen.logicalHeight(); y++)
	{
		for (int x = 0; x < screen.logicalWidth(); x++)
		{
			if (placedFigures[y][x])
			{
				screen.putSymb(SQUARE, { x, y });
			}
		}
	}
}