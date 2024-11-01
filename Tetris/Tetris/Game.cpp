#include "Tetris.h"
void Game::update()
{
    drawBorders();

    if (currentFigure.canMove(Direction::DOWN, placedFigures))
    {
        currentFigure.move(Direction::DOWN, placedFigures);
    }
    else
    {
        heap.placeFigure(currentFigure, placedFigures);           // Фиксируем фигуру
        heap.checkLines(placedFigures);            // Проверяем заполненные линии
        spawnFigure();          // Создаём новую фигуру
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
    //Heap.draw()
    heap.displayFigure(placedFigures);
    currentFigure.draw(screen);
    screen.draw();
}

void Game::dropFigure()
{
    while (currentFigure.canMove(Direction::DOWN, placedFigures))
    {
        currentFigure.move(Direction::DOWN, placedFigures);
    }
    heap.placeFigure(currentFigure, placedFigures);
    heap.checkLines(placedFigures);
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
            if (key == LEFT && currentFigure.canMove(Direction::LEFT, placedFigures))
            {
                currentFigure.move(Direction::LEFT, placedFigures);
            }
            else if (key == RIGHT && currentFigure.canMove(Direction::RIGHT, placedFigures))
            {
                currentFigure.move(Direction::RIGHT, placedFigures);
            }
        }
        else if (key == SPACE)
        {
            dropFigure();
        }
        else if (key == COUNTER_CLOCK)
        {
            currentFigure.rotate(Direction::LEFT, placedFigures);
        }
        else if (key == CLOCK)
        {
            currentFigure.rotate(Direction::RIGHT, placedFigures);
        }
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    }
}

void Game::drawBorders()
{
    // Проходим по всей высоте экрана
    for (int y = 0; y < SCREEN_HEIGHT; ++y)
    {
        // Если это верхняя или нижняя строка
        if (y == 0 || y == SCREEN_HEIGHT - 1)
        {

            drawSymb(' ', 0, y);  // Рисуем пробел в начале строки
            for (int x = 1; x < SCREEN_WIDTH - 1; ++x)
            {
                drawSymb('-', x, y);
            }
            drawSymb(' ', SCREEN_WIDTH - 1, y);  // Рисуем пробел в конце строки
        }
        else
        {
            // Для остальных строк рисуем вертикальные границы
            drawSymb('|', 0, y);                    // Левая граница
            drawSymb(' ', 1, y);                    // Отступ после левой границы
            drawSymb(' ', SCREEN_WIDTH - 2, y);     // Отступ перед правой границей
            drawSymb('|', SCREEN_WIDTH - 1, y);     // Правая граница
        }
    }
}

void Game::spawnFigure()
{
    vector<vector<vector<bool>>> shapes =
    {
        {{0,0,0,0,0},
         {0,0,0,0,0},
         {0,1,1,1,1},
         {0,0,0,0,0},
         {0,0,0,0,0}},
        {{0,0,0,0,0},
         {0,0,0,0,0},
         {0,1,1,1,0},
         {0,0,1,0,0},
         {0,0,0,0,0}},
        {{0,0,0,0,0},
         {0,0,1,1,0},
         {0,0,1,1,0},
         {0,0,0,0,0},
         {0,0,0,0,0}},
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
    currentFigure = Figure(shapes[randomIndex], { SCREEN_WIDTH / 4 - 2, 0 });
}