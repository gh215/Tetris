#include "Tetris.h"

void Heap::placeFigure(Figure& figure, vector<vector<bool>>& placedFigures)
{
	// Получаем текущую фигуру и её позицию
	vector<vector<bool>> shape = figure.getShape();
	Point pos = figure.getPosition();
	// Добавляем фигуру в массив размещённых фигур
	for (int row = 0; row < shape.size(); row++)
	{
		for (int col = 0; col < shape[row].size(); col++)
		{
			if (shape[row][col])
			{
				int x = pos.x + col;
				int y = pos.y + row;
				// Проверяем границы перед размещением
				if (y >= 0 && y < SCREEN_HEIGHT - 1 && x >= 0 && x < (SCREEN_WIDTH / 2 - 1))
				{
					placedFigures[y][x] = true;
				}
			}
		}
	}
}

// Проверка коллизий с фигурой
bool Heap::checkCollision(vector<vector<bool>>& screen, vector<vector<bool>>& shape, Point position)
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
				if (newX < 0 || newX >= SCREEN_WIDTH / 2 || newY >= SCREEN_HEIGHT)
				{
					return true; // Есть столкновение с границей
				}

				// Проверяем столкновение с другими фигурами
				if (newY >= 0 && screen[newY][newX])
				{
					return true; // Есть столкновение с другой фигурой
				}
			}
		}
	}
	return false; // Столкновений нет
}

// Проверка и удаление заполненных линий
void Heap::checkLines(vector<vector<bool>>& placedFigures)
{
	// Проверяем каждую строку снизу вверх
	for (int row = SCREEN_HEIGHT - 1; row >= 0; row--)
	{
		bool isLineFull = true;
		// Проверяем заполненность строки
		for (int col = 0; col < SCREEN_WIDTH / 2; col++)
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
				for (int x = 0; x < SCREEN_WIDTH / 2; x++)
				{
					placedFigures[y][x] = placedFigures[y - 1][x];
				}
			}
			// Очищаем верхнюю строку
			for (int x = 0; x < SCREEN_WIDTH / 2; x++)
			{
				placedFigures[0][x] = false;
			}
		}
	}
}

// Отрисовка всех блоков
void Heap::displayFigure(vector<vector<bool>>& placedFigures)
{
	for (int y = 0; y < SCREEN_HEIGHT; y++)
	{
		for (int x = 0; x < SCREEN_WIDTH / 2; x++)
		{
			if (placedFigures[y][x])
			{
				screen.putSymb(SQUARE, { x, y });
			}
		}
	}
}