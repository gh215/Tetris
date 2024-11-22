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
				// ��������� ������� ����� �����������
				if (y >= 0 && y < logicalHeight() - 1 && x >= 0 && x < (logicalWidth() - 1))
				{
					placedFigures[y][x] = true;
				}
			}
		}
	}

	game->figurePlaced();
}

bool Heap::checkCollision(vector<vector<bool>> shape, Point position)
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
				if (newX < 0 
					|| newX >= logicalWidth() 
					|| newY >= logicalHeight()
					|| (newY >= 0 && placedFigures[newY][newX]))
				{
					return true;
				}
			}
		}
	}
	return false; 
}

void Heap::checkLines()
{
	for (int row = logicalHeight() - 1; row >= 0; )
	{
		bool isLineFull = true;
		for (int col = 1; col < logicalWidth() - 1; col++)
		{
			if (!placedFigures[row][col])
			{
				isLineFull = false;
				break;
			}
		}

		if (isLineFull)
		{
			// �������� ������ � ����� ����� ���� ����
			for (int y = row; y > 0; y--)
			{
				for (int x = 0; x < logicalWidth(); x++)
				{
					placedFigures[y][x] = placedFigures[y - 1][x];
				}
			}
			for (int x = 1; x < logicalWidth() - 1; x++)
			{
				placedFigures[0][x] = false;
			}

			// ����������� ������� �������� �����
			game->totalLinesCleared();
		}
		else
		{
			row--;
		}
	}
}

// ��������� ���� ������
void Heap::draw()
{
	for (int y = 0; y < logicalHeight(); y++)
	{
		for (int x = 0; x < logicalWidth(); x++)
		{
			if (placedFigures[y][x])
			{
				screen.putSymb(SQUARE, screen.logicalToPhysicalX(x), y);
			}
		}
	}
}