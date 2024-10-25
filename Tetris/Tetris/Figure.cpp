#include "Tetris.h"

bool Figure::canMove(Direction dir, vector<vector<bool>>& field)
{
	// ������ ����� �������, ������� �������
	Point newPosition = position;

	// ���������� ����� ������� � ����������� �� �����������
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

	// ��������� ������ ������ ������
	for (size_t row = 0; row < shape.size(); row++)
	{
		for (size_t col = 0; col < shape[row].size(); col++)
		{
			// ���� � ������ ������� ���� ����� ������
			if (shape[row][col])
			{
				int newX = newPosition.x + col;
				int newY = newPosition.y + row;

				// ��������� ����� �� ������� ����
				if (newX < 1 || newX >= SCREEN_WIDTH / 2 - 1 || newY >= SCREEN_HEIGHT - 1)
				{
					return false;
				}

				// ��������� ������������ � ������� ��������
				if (newY >= 0 && field[newY][newX])
				{
					return false;
				}
			}
		}
	}
	return true; // ���� ��� �������� ��������, �������� ��������
}

bool Figure::canRotate(Direction dir, vector<vector<bool>>& field)
{
	// ������ ����� ������� ������
	vector<vector<bool>> rotatedShape = shape;
	int n = shape.size();

	// ������������ ����� � ������ �����������
	if (dir == Direction::LEFT)
	{
		rotatedShape = rotateCounterClockwise(shape);
	}
	else if (dir == Direction::RIGHT)
	{
		rotatedShape = rotateClockwise(shape);
	}

	// ��������� ����������� ��������
	for (int row = 0; row < rotatedShape.size(); row++)
	{
		for (int col = 0; col < rotatedShape[row].size(); col++)
		{
			// ���� � ������ ������� ���� ����� ������
			if (rotatedShape[row][col])
			{
				int newX = position.x + col;
				int newY = position.y + row;

				// ��������� ������� � ������������
				if (newX < 0 || newX >= SCREEN_WIDTH / 2 || newY >= SCREEN_HEIGHT || (newY >= 0 && field[newY][newX]))
				{
					return false;
				}
			}
		}
	}
	return true; // ���� ��� �������� ��������, ������� ��������
}

bool Figure::checkCollision(vector<vector<bool>>& field)
{
	// ��������� ������ ������ ������
	for (int row = 0; row < shape.size(); row++)
	{
		for (int col = 0; col < shape[row].size(); col++)
		{
			// ���� � ������ ������� ���� ����� ������
			if (shape[row][col])
			{
				int newX = position.x + col;
				int newY = position.y + row;

				// ��������� ����� �� �������
				if (newX < 0 || newX >= SCREEN_WIDTH / 2 || newY >= SCREEN_HEIGHT)
				{
					return true; // ���� ������������ � ��������
				}

				// ��������� ������������ � ������� ��������
				if (newY >= 0 && field[newY][newX])
				{
					return true; // ���� ������������ � ������ �������
				}
			}
		}
	}
	return false; // ������������ ���
}

void Figure::placeFigure()
{
	// �������� �� ���� ������� ������
	for (size_t row = 0; row < shape.size(); row++)
	{
		for (size_t col = 0; col < shape[row].size(); col++)
		{
			// ���� � ������ ������� ���� ����� ������
			if (shape[row][col])
			{
				int x = position.x + col;
				int y = position.y + row;

				// ��������� ������ ������ ���� ������ � ������� �������
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
