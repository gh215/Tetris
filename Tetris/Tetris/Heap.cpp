#include "Tetris.h"

void Heap::placeFigure(Figure& figure, vector<vector<bool>>& placedFigures)
{
	// �������� ������� ������ � � �������
	vector<vector<bool>> shape = figure.getShape();
	Point pos = figure.getPosition();
	// ��������� ������ � ������ ����������� �����
	for (int row = 0; row < shape.size(); row++)
	{
		for (int col = 0; col < shape[row].size(); col++)
		{
			if (shape[row][col])
			{
				int x = pos.x + col;
				int y = pos.y + row;
				// ��������� ������� ����� �����������
				if (y >= 0 && y < SCREEN_HEIGHT - 1 && x >= 0 && x < (SCREEN_WIDTH / 2 - 1))
				{
					placedFigures[y][x] = true;
				}
			}
		}
	}
}

// �������� �������� � �������
bool Heap::checkCollision(vector<vector<bool>>& screen, vector<vector<bool>>& shape, Point position)
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
				if (newY >= 0 && screen[newY][newX])
				{
					return true; // ���� ������������ � ������ �������
				}
			}
		}
	}
	return false; // ������������ ���
}

// �������� � �������� ����������� �����
void Heap::checkLines(vector<vector<bool>>& placedFigures)
{
	// ��������� ������ ������ ����� �����
	for (int row = SCREEN_HEIGHT - 1; row >= 0; row--)
	{
		bool isLineFull = true;
		// ��������� ������������� ������
		for (int col = 0; col < SCREEN_WIDTH / 2; col++)
		{
			if (!placedFigures[row][col])
			{
				isLineFull = false;
				break;
			}
		}
		// ���� ������ ���������
		if (isLineFull)
		{
			// �������� ��� ������ ���� ������� ����
			for (int y = row; y > 0; y--)
			{
				for (int x = 0; x < SCREEN_WIDTH / 2; x++)
				{
					placedFigures[y][x] = placedFigures[y - 1][x];
				}
			}
			// ������� ������� ������
			for (int x = 0; x < SCREEN_WIDTH / 2; x++)
			{
				placedFigures[0][x] = false;
			}
		}
	}
}

// ��������� ���� ������
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