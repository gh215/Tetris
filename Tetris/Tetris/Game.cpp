#include "Tetris.h"
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
}

void Game::draw(Screen& screen)
{
    heap.draw();
    currentFigure.draw(screen);
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
    Clock horizontalMoveClock;
    if (_kbhit())
    {
        int key = _getch();
        if (key == ARROW)
        {
            key = _getch();
            if (key == LEFT && currentFigure.canMove(Direction::LEFT, heap.getPlacedFigures()))
            {
                currentFigure.move(Direction::LEFT, heap.getPlacedFigures());
            }
            else if (key == RIGHT && currentFigure.canMove(Direction::RIGHT, heap.getPlacedFigures()))
            {
                currentFigure.move(Direction::RIGHT, heap.getPlacedFigures());

            }
        }
        else if (key == SPACE)
        {
            dropFigure();
        }
        else if (key == COUNTER_CLOCK)
        {
            currentFigure.rotate(Direction::LEFT, heap.getPlacedFigures());
        }
        else if (key == CLOCK)
        {
            currentFigure.rotate(Direction::RIGHT, heap.getPlacedFigures());
        }
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    }
}

void Game::drawBorders()
{
    screen.drawRect(0, 0, screen.logicalWidth(), screen.logicalHeight(), { '-', '|' });
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

    int randomIndex = rand() % shapes.size();
    currentFigure = Figure(shapes[randomIndex], { screen.logicalWidth() / 2 - 2 - 2, 0 });
}