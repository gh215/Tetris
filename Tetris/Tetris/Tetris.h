#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <Windows.h>
#include <conio.h>
#include <list>

using namespace std;

struct Point
{
	int x;
	int y;
};

typedef pair<char, char> symbol;

class Screen;
class Figure;
class Game;

enum class Direction { DOWN, LEFT, RIGHT };

const int SCREEN_HEIGHT = 30;
const int SCREEN_WIDTH = 30;
const symbol SQUARE = { '[', ']' };
const char EMPTY_CELL = ' ';
const int ARROW = 224;
const int LEFT = 75;
const int RIGHT = 77;
const int CLOCK = 'e';
const int COUNTER_CLOCK = 'q';
const int SLEEP = 300;


class Screen
{
private:
	char nextBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
	char activeBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
	void drawSymb(char c, size_t x, size_t y)
	{
		static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD coord = { (SHORT)x, (SHORT)y };
		SetConsoleCursorPosition(hOut, coord);
		cout << c;
	}
	void showConsoleCursor(bool showFlag);
public:
	Screen() { clear(); showConsoleCursor(false); }
	bool putSymb(symbol symb, Point p)
	{
		if (p.y < 0 || p.y > SCREEN_HEIGHT - 1) return false;
		if (p.x < 0 || p.x > SCREEN_WIDTH / 2 - 1) return false;
		if (!isprint(symb.first) || !isprint(symb.second)) return false;

		nextBuffer[p.y][p.x * 2] = symb.first;
		nextBuffer[p.y][p.x * 2 + 1] = symb.second;
		return true;
	}
	void putMatrix(vector<vector<bool>> m, Point p, const symbol symb)
	{
		for (int row = 0; row < m.size(); row++)
		{
			for (int col = 0; col < m.size(); col++)
			{
				if (m[row][col]) putSymb(symb, { p.x + col, p.y + row });
			}
		}
	}
	void draw()
	{
		for (size_t x = 0; x < SCREEN_WIDTH; x++)
		{
			for (size_t y = 0; y < SCREEN_HEIGHT; y++)
			{
				if (nextBuffer[y][x] == activeBuffer[y][x]) continue;
				activeBuffer[y][x] = nextBuffer[y][x];
				drawSymb(activeBuffer[y][x], x, y);
			}
			
		}
		clear();
	}
	void clear()
	{
		for (size_t y = 0; y < SCREEN_WIDTH; y++)
		{
			for (size_t x = 0; x < SCREEN_HEIGHT; x++)
			{
				nextBuffer[y][x] = EMPTY_CELL;

			}
		}
	}
	void drawBorders();
};

class Figure
{
private:
	vector<vector<bool>> shape;
	Point position;
public:
	Figure(vector<vector<bool>> shape, Point pos) : shape(shape), position(pos) {}
	void draw(Screen& screen)
	{
		screen.putMatrix(shape, position, SQUARE);
	}
	void rotate(Direction dir)
	{
		if (dir == Direction::LEFT)
		{

		}
		else if (dir == Direction::RIGHT)
		{

		}
	}
	void move(Direction dir);
};

class Game
{
private:
	Screen field;
	Figure currentFigure;
	Point position;
	bool isGameOver;
public:
	Game() : field(), isGameOver(false), currentFigure({ {1, 1, 1, 1} }, { 0, 0 }), position({0,0}) {}
	void spawnFigure();
	void draw(Screen& field);

	void moveFigure(Direction dir) { currentFigure.move(dir); }
	void rotateFigure(Direction dir) { currentFigure.rotate(dir); }

	vector<vector<int>> rotateClockwise(const vector<vector<int>>& shape);
	vector<vector<int>> rotateCounterClockwise(const vector<vector<int>>& shape);

	void run();
	void processInput();
	void update();
};


