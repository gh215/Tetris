#include "Tetris.h"

Clock globalClock;

void Game::restart()
{
	currentFigure = Figure(vector<vector<bool>>(), { 0, 0 });
	nextFigure = Figure(vector<vector<bool>>(), { 0, 0 });
	//heap = Heap(screen, this);
	position = { 0, 0 };
	isGameOver = false;
	isFastFall = false;
	linesCleared = 0;
	score = 0;

	spawnFigure();
}

void Game::update()
{
	drawBorders();
	if (currentFigure.canMove(Direction::DOWN, heap.getPlacedFigures()))
	{
		currentFigure.move(Direction::DOWN, heap.getPlacedFigures());
	}
	else
	{
		heap.placeFigure(currentFigure);
		heap.checkLines();
		spawnFigure();
		if (heap.checkCollision(currentFigure.getShape(), currentFigure.getPosition()))
		{
			isGameOver = true;
			screen.showGameOverMessage();
			screen.moveCursorToBottom();
		}
	}
}
void Game::run()
{
	spawnFigure();
	while (!isGameOver)
	{
		processInput();
		update();
		draw(screen);
		Sleep(SLEEP);
	}
	waitForInput();
}

void Game::draw(Screen& screen)
{
	heap.draw();
	currentFigure.draw(screen);
	drawInfoBoxes();
	screen.draw();
}

void Game::dropFigure()
{
	while (currentFigure.canMove(Direction::DOWN, heap.getPlacedFigures()))
	{
		currentFigure.move(Direction::DOWN, heap.getPlacedFigures());
	}
	heap.placeFigure(currentFigure);
	heap.checkLines();
	spawnFigure();
}


void Game::processInput()
{
	if (_kbhit())
	{
		int key = _getch();
		if (key == ARROW)
		{
			key = _getch();
			if (key == LEFT && currentFigure.canMove(Direction::LEFT, heap.getPlacedFigures()))
			{
				if (globalClock.getTicks() % 20 == 0)
				{
					currentFigure.move(Direction::LEFT, heap.getPlacedFigures());
				}

			}
			else if (key == RIGHT && currentFigure.canMove(Direction::RIGHT, heap.getPlacedFigures()))
			{
				if (globalClock.getTicks() % 20 == 0)
				{
					currentFigure.move(Direction::RIGHT, heap.getPlacedFigures());
				}
			}
		}
		else if (key == PAUSE_LOWER || key == PAUSE_UPPER)
		{
			checkPause();
		}
		else if (key == SPACE)
		{
			dropFigure();
		}
		if (key == RESTART_LOWERCASE || key == RESTART_UPPERCASE)
		{
			restart();
			return;
		}
		else if (key == QUIT_LOWERCASE || key == QUIT_UPPERCASE)
		{
			exit(0);
		}
		else if (key == COUNTER_CLOCK_LOWERCASE || key == COUNTER_CLOCK_UPPERCASE)
		{
			currentFigure.rotate(Direction::LEFT, heap.getPlacedFigures());
		}
		else if (key == CLOCK_LOWERCASE || key == CLOCK_UPPERCASE)
		{
			currentFigure.rotate(Direction::RIGHT, heap.getPlacedFigures());
		}
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	}
}

void Game::drawBorders()
{
	screen.drawRect(0, 0, logicalWidth(), logicalHeight(), { '-', '|' });;
}

void Game::checkPause()
{
	screen.showPauseMessage();
	int ignore = _getch();
	screen.clearPauseMessage();
	drawBorders();
	draw(screen);
}

void Game::spawnFigure()
{
	vector<vector<vector<bool>>> shapes =
	{
		{{0,0,0,0},
		 {0,0,0,0},
		 {1,1,1,1},
		 {0,0,0,0}},
		{{0,0,0,0,0},
		 {0,0,0,0,0},
		 {0,1,1,1,0},
		 {0,0,1,0,0},
		 {0,0,0,0,0}},
		{{0,0,0,0,},
		 {0,1,1,0,},
		 {0,1,1,0,},
		 {0,0,0,0,}},
		{{0,0,0,0,0},
		 {0,0,1,0,0},
		 {0,0,1,0,0},
		 {0,0,1,1,0},
		 {0,0,0,0,0}},
		{{0,0,0,0,0},
		 {0,0,1,0,0},
		 {0,0,1,0,0},
		 {0,1,1,0,0},
		 {0,0,0,0,0}},
		{{0,0,0,0,0},
		 {0,0,0,0,0},
		 {0,0,1,1,0},
		 {0,1,1,0,0},
		 {0,0,0,0,0}},
		{{0,0,0,0,0},
		 {0,0,0,0,0},
		 {0,1,1,0,0},
		 {0,0,1,1,0},
		 {0,0,0,0,0}}
	};

	if (currentFigure.getShape().empty())
	{
		int randomIndex = rand() % shapes.size();
		currentFigure = Figure(shapes[randomIndex], { (GAME_FIELD_WIDTH / 4) - 2, 0 });
	}
	else
	{
		currentFigure = nextFigure;
	}

	int randomIndex = rand() % shapes.size();
	nextFigure = Figure(shapes[randomIndex], { (GAME_FIELD_WIDTH / 4) - 2, 0 });
}

void Game::displayNextFigure()
{
	vector<vector<bool>> shape = nextFigure.getShape();
	Point pos;
	pos.x = GAME_FIELD_WIDTH / 2 + 5;
	pos.y = 5;
	screen.putMatrix(shape, pos, SQUARE);
}

void Game::drawInfoBoxes()
{
	// Окно для строк (Lines)
	screen.drawRect(GAME_FIELD_WIDTH / 2 + 1, 1, 8, 3, { '-', '|' });
	string linesText = "LINES: " + to_string(linesCleared);
	screen.putText(linesText, GAME_FIELD_WIDTH / 2 + 2, 2);

	// Окно для следующей фигуры (Next Figure)
	screen.drawRect(GAME_FIELD_WIDTH / 2 + 1, 5, 10, 5, { '-', '|' });
	screen.putText("NEXT:", GAME_FIELD_WIDTH / 2 + 2, 7);
	displayNextFigure();

	// Окно для очков (Score)
	screen.drawRect(GAME_FIELD_WIDTH / 2 + 1, 12, 9, 3, { '-', '|' });
	string scoreText = "SCORE: " + to_string(score);
	screen.putText(scoreText, GAME_FIELD_WIDTH / 2 + 2, 13);

	// Справочная информация
	screen.drawRect(GAME_FIELD_WIDTH / 2 + 1, 17, 13, 8, { '-', '|' });
	screen.putText("CONTROLS:", GAME_FIELD_WIDTH / 2 + 2, 18);
	screen.putText("Right and Left: Move", GAME_FIELD_WIDTH / 2 + 2, 19);
	screen.putText("R: Restart", GAME_FIELD_WIDTH / 2 + 2, 20);
	screen.putText("Q/E: Rotate", GAME_FIELD_WIDTH / 2 + 2, 21);
	screen.putText("SPACE: Drop", GAME_FIELD_WIDTH / 2 + 2, 22);
	screen.putText("P: Pause", GAME_FIELD_WIDTH / 2 + 2, 23);
}
