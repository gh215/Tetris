#pragma once
#pragma warning(disable: 4018)
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <Windows.h>
#include <conio.h>
#include <list>

using namespace std;

const int FIELD_HEIGHT = 30;
const int FIELD_WIDTH = 44;
const int SLEEP = 200;
const int HORIZONTAL_MOVE_SLEEP = 50;

const char EMPTY_CELL = ' ';
const int ARROW = 224;
const int LEFT = 75;
const int SPACE = 32;
const int RIGHT = 77;
const int DOWN = 80;
const int CLOCK = 'e';
const int COUNTER_CLOCK = 'q';

struct Point
{
	int x;
	int y;
};

typedef pair<char, char> symbol;

enum class Direction { DOWN, LEFT, RIGHT };

const symbol SQUARE = { '[', ']' };

class Screen;
class Figure;
class Game;
class Heap;

class Clock 
{
private:
	long ticks;
public:
	Clock() : ticks(0) {}
	void tick() { ticks++; }
	long getTicks() { return ticks; }
};

class Screen
{
private:
	char nextBuffer[FIELD_HEIGHT][FIELD_WIDTH];
	char activeBuffer[FIELD_HEIGHT][FIELD_WIDTH];

	void drawSymb(char c, int x, int y)
	{
		static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD coord = { (SHORT)x, (SHORT)y };
		SetConsoleCursorPosition(hOut, coord);
		cout << c;
	}
	void showConsoleCursor(bool showFlag);
public:
	Screen() 
	{ 
		clear();
		showConsoleCursor(false); 
	}
	int logicalWidth() const { return FIELD_WIDTH / 2; }
	int logicalHeight() const { return FIELD_HEIGHT; }

	int logicalToPhysicalX(int x) const { return x * 2; }
	int logicalToPhysicalY(int y) const { return y; }
	bool putSymb(symbol symb, Point p)
	{
		if (p.y < 0 || p.y > logicalHeight() - 1) return false;
		if (p.x < 0 || p.x >= logicalWidth() - 1) return false;
		if (!isprint(symb.first) || !isprint(symb.second)) return false;

		int physicalX = logicalToPhysicalX(p.x);
		nextBuffer[p.y][physicalX] = symb.first;
		nextBuffer[p.y][physicalX + 1] = symb.second;
		return true;
	}
	void putMatrix(vector<vector<bool>> m, Point p, const symbol symb)
	{
		for (int row = 0; row < m.size(); row++)
		{
			for (int col = 0; col < m[row].size(); col++)
			{
				if (m[row][col]) putSymb(symb, { p.x + col, p.y + row });
			}
		}
	}
	void draw()
	{
		for (size_t x = 0; x < FIELD_WIDTH; x++)
		{
			for (size_t y = 0; y < FIELD_HEIGHT; y++)
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
		for (size_t y = 0; y < FIELD_HEIGHT; y++) 
		{
			for (size_t x = 0; x < FIELD_WIDTH; x++) 
			{
				nextBuffer[y][x] = EMPTY_CELL;
			}
		}
	}
	void drawRect(int x, int y, int width, int height, symbol borde);
};

class Figure
{
private:
	vector<vector<bool>> shape;
	Point position;
public:
	Figure(vector<vector<bool>> shape, Point pos) : shape(shape), position(pos) {}

	vector<vector<bool>> rotate(bool clockwise);
	bool checkPosition(const vector<vector<bool>>& testShape, Point testPos, const vector<vector<bool>>& screen);

	vector<vector<bool>> getShape() const { return shape; }
	Point getPosition() const { return position; }

	void draw(Screen& screen)
	{
		screen.putMatrix(shape, position, SQUARE);
	}
	void rotate(Direction dir, vector<vector<bool>> heap)
	{
		if (canRotate(dir, heap))
		{
			if (dir == Direction::LEFT) 
			{
				shape = rotate(false);
			}
			else if (dir == Direction::RIGHT) 
			{
				shape = rotate(true);
			}
		}
	}
	void move(Direction dir, vector<vector<bool>> heap)
	{
		if (canMove(dir, heap))
		{
			switch (dir)
			{
			case Direction::DOWN:
				position.y++;
				break;
			case Direction::LEFT:
				position.x--;
				break;
			case Direction::RIGHT:
				position.x++;
				break;
			}
		}
	}

	bool canMove(Direction dir, vector<vector<bool>> heap);
	bool canRotate(Direction dir, vector<vector<bool>> heap);
};

class Heap
{
private:
	vector<vector<bool>> blocks;
	vector<vector<bool>> placedFigures;
	Screen& screen;
public:
	Heap(Screen& scr) : screen(scr)
	{
		blocks.resize(screen.logicalHeight(), vector<bool>(screen.logicalWidth(), false));
		placedFigures.resize(screen.logicalHeight(), vector<bool>(screen.logicalWidth(), false));
	}

	void placeFigure(Figure& figure);
	bool checkCollision(vector<vector<bool>> shape, Point position);
	void checkLines();
	void draw();

	vector<vector<bool>> getPlacedFigures() const { return placedFigures; }
};
 
class Game
{
private:
	Screen screen;
	Figure currentFigure;
	Point position;
	Heap heap;
	Clock gameClock;
	bool isGameOver;
	bool isFastFall;
public:
	Game() : screen(),
		currentFigure({ {1, 1, 1, 1} }, { 0, 0 }),
		heap(screen),
		position({0,0}),
		isGameOver(false),
		isFastFall(false) {}
	void spawnFigure();
	void draw(Screen& screen);

	void moveFigure(Direction dir) { currentFigure.move(dir, heap.getPlacedFigures()); }
	void rotateFigure(Direction dir) { currentFigure.rotate(dir, heap.getPlacedFigures()); }
	void drawBorders();

	void run();
	void processInput();
	void update();

	void dropFigure();
};


