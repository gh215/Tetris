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

struct Point
{
	int x;
	int y;
};

typedef pair<char, char> symbol;

class Screen;
class Figure;
class Game;
class Heap;

enum class Direction { DOWN, LEFT, RIGHT };

const int SCREEN_HEIGHT = 30;
const int SCREEN_WIDTH = 44;
const symbol SQUARE = { '[', ']' };
const char EMPTY_CELL = ' ';
const int ARROW = 224;
const int LEFT = 75;
const int SPACE = 32;
const int RIGHT = 77;
const int DOWN = 80;
const int CLOCK = 'e';
const int COUNTER_CLOCK = 'q';
const int SLEEP = 200;
const uint64_t HORIZONTAL_SLEEP = 50;  


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
	int logicalToPhysicalX(int x) const { return x * 2; }
public:
	Screen() { clear(); showConsoleCursor(false); }
	bool putSymb(symbol symb, Point p)
	{
		if (p.y < 0 || p.y > SCREEN_HEIGHT - 1) return false;
		if (p.x < 0 || p.x > SCREEN_WIDTH / 2 - 1) return false;
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
		for (size_t y = 0; y < SCREEN_HEIGHT; y++) 
		{
			for (size_t x = 0; x < SCREEN_WIDTH; x++) 
			{
				nextBuffer[y][x] = EMPTY_CELL;
			}
		}
	}
	void drawRect(int x, int y, int width, int height, symbol border);
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
	void rotate(Direction dir, vector<vector<bool>>& screen)
	{
		if (canRotate(dir, screen)) 
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
	void move(Direction dir, vector<vector<bool>>& screen)
	{
		if (canMove(dir, screen))
		{
			switch (dir) {
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

	bool canMove(Direction dir, vector<vector<bool>>& screen);
	bool canRotate(Direction dir, vector<vector<bool>>& screen);
};

class Heap
{
private:
	vector<vector<bool>> blocks;
	Screen& screen;
public:
	Heap(Screen& scr) : screen(scr)
	{
		blocks.resize(SCREEN_HEIGHT, vector<bool>(SCREEN_WIDTH / 2, false));
	}

	void placeFigure(Figure& figure, vector<vector<bool>>& placedFigures);
	bool checkCollision(vector<vector<bool>>& screen, vector<vector<bool>>& shape, Point position);
	void checkLines(vector<vector<bool>>& placedFigures);
	void displayFigure(vector<vector<bool>>& placedFigures);
};
 
class Game
{
private:
	Screen screen;
	Figure currentFigure;
	Point position;
	Heap heap;
	vector<vector<bool>> placedFigures;
	bool isGameOver;
	bool isFastFall;
	void drawSymb(char c, size_t x, size_t y)
	{
		static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD coord = { (SHORT)x, (SHORT)y };
		SetConsoleCursorPosition(hOut, coord);
		cout << c;
	}
public:
	Game() : screen(),
		currentFigure({ {1, 1, 1, 1} }, { 0, 0 }),
		heap(screen),
		position({0,0}),
		isGameOver(false),
		isFastFall(false),
		placedFigures(SCREEN_HEIGHT, vector<bool>(SCREEN_WIDTH / 2, false)) {}
	void spawnFigure();
	void draw(Screen& screen);

	void moveFigure(Direction dir) { currentFigure.move(dir, placedFigures); }
	void rotateFigure(Direction dir) { currentFigure.rotate(dir, placedFigures); }
	void drawBorders();

	void run();
	void processInput();
	void update();

	void dropFigure();
};


