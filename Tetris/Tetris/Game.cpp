#include "Tetris.h"
void Game::update()
{
    // Рисуем границы поля
    field.drawBorders();
    // Проверяем возможность движения вниз
    if (currentFigure.canMove(Direction::DOWN, placedFigures))
    {
        currentFigure.move(Direction::DOWN, placedFigures);
    }
    else
    {
        // Если движение вниз невозможно:
        placeFigure();           // Фиксируем фигуру
        checkLines();            // Проверяем заполненные линии
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
        draw(field);
        Sleep(SLEEP);
    }
}
void Game::draw(Screen& field)
{
    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH / 2; x++)
        {
            if (placedFigures[y][x])
            {
                field.putSymb(SQUARE, { x, y });
            }
        }
    }
    currentFigure.draw(field);
    field.draw();
}
void Game::processInput()
{
    if (_kbhit())
    {
        int key = _getch();
        if (key == ARROW)
        {
            key = _getch();
            // Обработка движения влево
            if (key == LEFT && currentFigure.canMove(Direction::LEFT, placedFigures))
            {
                currentFigure.move(Direction::LEFT, placedFigures);
            }
            // Обработка движения вправо
            else if (key == RIGHT && currentFigure.canMove(Direction::RIGHT, placedFigures))
            {
                currentFigure.move(Direction::RIGHT, placedFigures);
            }
        }
        // Обработка поворота против часовой стрелки
        else if (key == COUNTER_CLOCK)
        {
            currentFigure.rotate(Direction::LEFT, placedFigures);
        }
        // Обработка поворота по часовой стрелке
        else if (key == CLOCK)
        {
            currentFigure.rotate(Direction::RIGHT, placedFigures);
        }
    }
}
void Game::placeFigure()
{
    // Получаем текущую фигуру и её позицию
    vector<vector<bool>> shape = currentFigure.getShape();
    Point pos = currentFigure.getPosition();
    // Добавляем фигуру в массив размещённых фигур
    for (int row = 0; row < shape.size(); row++)
    {
        for (int col = 0; col < shape[row].size(); col++)
        {
            if (shape[row][col])
            {
                int x = pos.x + col;
                int y = pos.y + row;
                // Проверяем границы перед размещением
                if (y >= 0 && y < SCREEN_HEIGHT - 1 && x >= 0 && x < (SCREEN_WIDTH / 2 - 1))
                {
                    placedFigures[y][x] = true;
                }
            }
        }
    }
}
void Game::checkLines()
{
    // Проверяем каждую строку снизу вверх
    for (int row = SCREEN_HEIGHT - 1; row >= 0; row--)
    {
        bool isLineFull = true;
        // Проверяем заполненность строки
        for (int col = 0; col < SCREEN_WIDTH / 2; col++)
        {
            if (!placedFigures[row][col])
            {
                isLineFull = false;
                break;
            }
        }
        // Если строка заполнена
        if (isLineFull)
        {
            // Сдвигаем все строки выше текущей вниз
            for (int y = row; y > 0; y--)
            {
                for (int x = 0; x < SCREEN_WIDTH / 2; x++)
                {
                    placedFigures[y][x] = placedFigures[y - 1][x];
                }
            }
            // Очищаем верхнюю строку
            for (int x = 0; x < SCREEN_WIDTH / 2; x++)
            {
                placedFigures[0][x] = false;
            }
        }
    }
}