#include "Tetris.h"

bool Figure::canMove(Direction dir, vector<vector<bool>>& screen)
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

	return checkPosition(shape, newPosition, screen);
}

bool Figure::canRotate(Direction dir, vector<vector<bool>>& screen)
{
	vector<vector<bool>> rotated = rotate(dir == Direction::RIGHT);
	return checkPosition(rotated, position, screen);
}

vector<vector<bool>> Figure::rotate(bool clockwise) 
{
	int n = shape.size();
	vector<vector<bool>> rotated(n, vector<bool>(n));

	for (int i = 0; i < n; i++) 
	{
		for (int j = 0; j < n; j++) 
		{
			if (clockwise) 
			{
				rotated[j][n - 1 - i] = shape[i][j];
			}
			else 
			{
				rotated[n - 1 - j][i] = shape[i][j];
			}
		}
	}
	return rotated;
}

bool Figure::checkPosition(const vector<vector<bool>>& shape, Point pos, const vector<vector<bool>>& screen) 
{
	for (int row = 0; row < shape.size(); row++) 
	{
		for (int col = 0; col < shape[row].size(); col++) 
		{
			if (shape[row][col]) 
			{
				int newX = pos.x + col;
				int newY = pos.y + row;

				if (newX < 1 || newX >= SCREEN_WIDTH / 2 - 1 || newY >= SCREEN_HEIGHT - 1 || (newY >= 0 && screen[newY][newX]))
				{
					return false;
				}
			}
		}
	}
	return true;
}
