#include "Tetris.h"

bool Figure::canMove(Direction dir, vector<vector<bool>>& field)
{
	// Создаём новую позицию, копируя текущую
	Point newPosition = position;

	// Определяем новую позицию в зависимости от направления
	switch (dir)
	{
	case Direction::DOWN:
		newPosition.y++;      
		break;
	case Direction::LEFT:
		newPosition.x--;      
		break;
	case Direction::RIGHT:
		newPosition.x++;      
		break;
	}

	// Проверяем каждую ячейку фигуры
	for (size_t row = 0; row < shape.size(); row++)
	{
		for (size_t col = 0; col < shape[row].size(); col++)
		{
			// Если в данной позиции есть часть фигуры
			if (shape[row][col])
			{
				int newX = newPosition.x + col;
				int newY = newPosition.y + row;

				// Проверяем выход за границы поля
				if (newX < 1 || newX >= SCREEN_WIDTH / 2 - 1 || newY >= SCREEN_HEIGHT - 1)
				{
					return false;
				}

				// Проверяем столкновение с другими фигурами
				if (newY >= 0 && field[newY][newX])
				{
					return false;
				}
			}
		}
	}
	return true; // Если все проверки пройдены, движение возможно
}

bool Figure::canRotate(Direction dir, vector<vector<bool>>& field)
{
	// Создаём копию текущей фигуры
	vector<vector<bool>> rotatedShape = shape;
	int n = shape.size();

	// Поворачиваем копию в нужном направлении
	if (dir == Direction::LEFT)
	{
		rotatedShape = rotateCounterClockwise(shape);
	}
	else if (dir == Direction::RIGHT)
	{
		rotatedShape = rotateClockwise(shape);
	}

	// Проверяем возможность поворота
	for (int row = 0; row < rotatedShape.size(); row++)
	{
		for (int col = 0; col < rotatedShape[row].size(); col++)
		{
			// Если в данной позиции есть часть фигуры
			if (rotatedShape[row][col])
			{
				int newX = position.x + col;
				int newY = position.y + row;

				// Проверяем границы и столкновения
				if (newX < 0 || newX >= SCREEN_WIDTH / 2 || newY >= SCREEN_HEIGHT || (newY >= 0 && field[newY][newX]))
				{
					return false;
				}
			}
		}
	}
	return true; // Если все проверки пройдены, поворот возможен
}

bool Figure::checkCollision(vector<vector<bool>>& field)
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
				if (newY >= 0 && field[newY][newX])
				{
					return true; // Есть столкновение с другой фигурой
				}
			}
		}
	}
	return false; // Столкновений нет
}

void Figure::placeFigure()
{
	// Проходим по всем ячейкам фигуры
	for (size_t row = 0; row < shape.size(); row++)
	{
		for (size_t col = 0; col < shape[row].size(); col++)
		{
			// Если в данной позиции есть часть фигуры
			if (shape[row][col])
			{
				int x = position.x + col;
				int y = position.y + row;

				// Размещаем символ только если фигура в видимой области
				if (y >= 0)
				{
					screen.putSymb(SQUARE, { x, y });
				}
			}
		}
	}
}

vector<vector<bool>> Figure::rotateClockwise(vector<vector<bool>>& shape)
{
    int rows = shape.size();
    int cols = shape[0].size();
    vector<vector<bool>> rotated(cols, vector<bool>(rows));

    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j)
        {
            rotated[j][rows - 1 - i] = shape[i][j];
        }
    }

    return rotated;
}

vector<vector<bool>> Figure::rotateCounterClockwise(vector<vector<bool>>& shape)
{
    int rows = shape.size();
    int cols = shape[0].size();
    vector<vector<bool>> rotated(cols, vector<bool>(rows));

    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j)
        {
            rotated[cols - 1 - j][i] = shape[i][j];
        }
    }

    return rotated;
}
