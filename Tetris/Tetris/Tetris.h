#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <conio.h>
#include <list>

enum class Direction { DOWN, LEFT, RIGHT };

using namespace std;

const int FIELD_WIDTH = 35;
const int FIELD_HEIGHT = 40;
const char EMPTY_CELL = ' ';
const char FILLED_CELL_LEFT = '[';
const char FILLED_CELL_RIGHT = ']';
const int ARROW = 224;
const int LEFT = 75;
const int RIGHT = 77;
const int CLOCK = 'e';
const int COUNTER_CLOCK = 'q';
const int SLEEP = 300;

struct Point
{
	int x;
	int y;
};

class GameField;

class Figure
{
private:
	vector<vector<int>> shape;
	Point position;
public:
	Figure(vector<vector<int>> shape) : shape(shape), position({ 0, 0 }) {}
	void move(Direction dir);
	void draw(GameField& field);
	void rotate(Direction dir);
	vector<vector<int>> rotateClockwise(const vector<vector<int>>& shape);
	vector<vector<int>> rotateCounterClockwise(const vector<vector<int>>& shape);
	void setStartPosition(int x, int y)
	{
		position.x = x;
		position.y = y;
	}
};

class GameField
{
private:
	char nextBuffer[FIELD_HEIGHT][FIELD_WIDTH];
	char activeBuffer[FIELD_HEIGHT][FIELD_WIDTH];
	Figure currentFigure;
	Figure nextFigure;
	void drawSymb(char symb, size_t x, size_t y)
	{
		static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD coord = { (SHORT)x, (SHORT)y };
		SetConsoleCursorPosition(hOut, coord);
		cout << symb;
	}
	int score;
	int lines;
	void showConsoleCursor(bool showFlag);
public:
	GameField() : currentFigure({ {1, 1, 1, 1} }), nextFigure({ {1, 1, 1, 1} }), score(0), lines(0)
	{
		clear();
		showConsoleCursor(false);
	}
	bool put(char symb, Point p)
	{
		if (p.x < 0 || p.x >= FIELD_WIDTH) return false;
		if (p.y < 0 || p.y >= FIELD_HEIGHT) return false;
		if (!isprint(symb)) return false;

		nextBuffer[p.y][p.x] = symb;
		return true;
	}
	void draw();
	void rotateFigure(Direction dir)
	{
		currentFigure.rotate(dir);
	}
	void clear()
	{
		for (size_t y = 0; y < FIELD_WIDTH; y++)
		{
			for (size_t x = 0; x < FIELD_HEIGHT; x++)
			{
				nextBuffer[y][x] = EMPTY_CELL;
			}
		}
	}
	void spawnFigure();
	void moveFigure(Direction dir) { currentFigure.move(dir); }
	bool checkCollision();
	void fixFigure();
	void clearLines();
	bool isGameOver();
	void drawBorders();
	int getScore() { return score; }
	int getLines() { return lines; }
};

class Game
{
private:
	GameField field;
	bool isGameOver;
public:
	Game() : field(), isGameOver(false) {}
	void run();
	void processInput();
	void update();
};


