#include "Tetris.h"

void GameField::showConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(out, &cursorInfo);
}

void GameField::drawBorders()
{
    for (int x = 0; x < FIELD_WIDTH; ++x)
    {
        put('-', { x, 0 });
    }

    for (int y = 1; y < FIELD_HEIGHT - 1; ++y)
    {
        put('|', { 0, y });
        put('|', { FIELD_WIDTH - 1, y });
    }

    for (int x = 0; x < FIELD_WIDTH; ++x)
    {
        put('-', { x, FIELD_HEIGHT - 1 });
    }

}

void GameField::spawnFigure()
{
    vector<vector<int>> shapes[] =
    {
        {{1, 1, 1, 1}},              // A - I-образная
        {{1, 1, 1}, {0, 0, 1}},      // B - L-образная
        {{1, 1, 1}, {0, 1, 0}}       // C - T-образная
    };

    int randomIndex = rand() % 3;
    currentFigure = Figure(shapes[randomIndex]);

    currentFigure.setStartPosition(FIELD_WIDTH / 2 - 2, 0);
}

void GameField::draw()
{
    clear();
    drawBorders();
    currentFigure.draw(*this);
    for (size_t x = 0; x < FIELD_WIDTH; x++)
    {
        for (size_t y = 0; y < FIELD_HEIGHT; y++)
        {
            if (nextBuffer[y][x] == activeBuffer[y][x]) continue;
            activeBuffer[y][x] = nextBuffer[y][x];
            drawSymb(activeBuffer[y][x], x, y);
        }
    }
}

