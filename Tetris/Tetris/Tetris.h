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
const int FIELD_WIDTH = 70;  
const int GAME_FIELD_WIDTH = 44; 
const int INFO_START_X = 46;  
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
const int PAUSE_LOWER = 112;
const int PAUSE_UPPER = 80;

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

inline int logicalWidth() { return GAME_FIELD_WIDTH / 2; };
inline int logicalHeight() { return FIELD_HEIGHT; };

class Clock 
{
private:
	long ticks;
public:
	Clock() : ticks(0) {}
	void tick() { ticks++; }
	long getTicks() { return ticks; }
};

extern Clock globalClock;

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

	int logicalToPhysicalX(int x) const { return x * 2; }
	bool putSymb(symbol symb, int physicalX, int physicalY)
	{
		if (physicalY < 0 || physicalY >= FIELD_HEIGHT) return false;
		if (physicalX < 0 || physicalX >= FIELD_WIDTH - 1) return false;
		if (!isprint(symb.first) || !isprint(symb.second)) return false;

		nextBuffer[physicalY][physicalX] = symb.first;
		nextBuffer[physicalY][physicalX + 1] = symb.second;
		return true;
	}
	void putMatrix(vector<vector<bool>> m, Point p, const symbol symb)
	{
		for (int row = 0; row < m.size(); row++)
		{
			for (int col = 0; col < m[row].size(); col++)
			{
				if (m[row][col]) putSymb(symb, logicalToPhysicalX(p.x + col), p.y + row);
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
	void putText(const string& text, int x, int y)
	{
		int px = logicalToPhysicalX(x);
		if (y < 0 || y >= FIELD_HEIGHT || px < 0 || px + text.length() >= FIELD_WIDTH) return;

		for (size_t i = 0; i < text.size(); i++)
		{
			if (px + i >= FIELD_WIDTH) break;  // Предотвращаем выход за пределы буфера
			nextBuffer[y][px + i] = text[i];
		}
	}
	void moveCursorToBottom();
	void drawRect(int x, int y, int width, int height, symbol border);
	void clearPauseMessage();
	void boardMessage(string message);
	void showPauseMessage();
	void showGameOverMessage();
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
	Game* game;
public:
	Heap(Screen& scr, Game* g) : screen(scr), game(g)
	{
		blocks.resize(logicalHeight(), vector<bool>(logicalWidth(), false));
		placedFigures.resize(logicalHeight(), vector<bool>(logicalWidth(), false));
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
	Figure nextFigure;
	Point position;
	Heap heap;
	Clock gameClock;
	bool isGameOver;
	bool isFastFall;
	int linesCleared;
	int score;
public:
	Game() : screen(),
		currentFigure(vector<vector<bool>>(), { 0, 0 }),  // Пустая фигура
		nextFigure(vector<vector<bool>>(), { 0, 0 }),     // Пустая фигура
		heap(screen, this),
		position({0,0}),
		isGameOver(false),
		isFastFall(false),
		linesCleared(0),
		score(0) {}

	void spawnFigure();
	void displayNextFigure();
	void draw(Screen& screen);

	void moveFigure(Direction dir) { currentFigure.move(dir, heap.getPlacedFigures()); }
	void rotateFigure(Direction dir) { currentFigure.rotate(dir, heap.getPlacedFigures()); }
	void drawBorders();

	void run();
	void processInput();
	void update();

	void dropFigure();

	void checkPause();
	void clearPauseMessage();
	void boardMessage(string message);
	void showPauseMessage();

	void drawInfoBoxes();
	void totalLinesCleared() 
	{ 
		linesCleared++; 
		addScore(100);
	}

	void waitForInput() { int ignore = _getch(); }

	void figurePlaced() { addScore(10); }

	void addScore(int points) { score += points; }
	int getScore() const { return score; }
};




