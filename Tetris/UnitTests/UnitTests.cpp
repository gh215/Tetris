#include "pch.h"
#include "CppUnitTest.h"
#include "..\Tetris\Tetris.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(UnitTests)
	{
	public:	
        TEST_METHOD(TestCanMove_NormalMovement)
        {
            vector<vector<bool>> shape = 
            {
                {1, 1},
                {1, 1}
            };

            Point startPos = { 5, 5 };
            vector<vector<bool>> heap(logicalHeight(), vector<bool>(logicalWidth(), false));
            Figure figure(shape, startPos);

            Assert::IsTrue(figure.canMove(Direction::DOWN, heap));
            Assert::IsTrue(figure.canMove(Direction::LEFT, heap));
            Assert::IsTrue(figure.canMove(Direction::RIGHT, heap));   
        }

        TEST_METHOD(TestCanMove_BarrierMovement)
        {
            vector<vector<bool>> shape =
            {
                {1, 1},
                {1, 1}
            };
            Point startPos = { 5, 5 };
            vector<vector<bool>> heap(logicalHeight(), vector<bool>(logicalWidth(), false));

            // Препятствия
            heap[6][5] = true;  // Блок снизу
            heap[5][4] = true;  // Блок слева
            heap[5][7] = true;  // Блок справа

            Figure figure(shape, startPos);

            Assert::IsFalse(figure.canMove(Direction::DOWN, heap));
            Assert::IsFalse(figure.canMove(Direction::LEFT, heap));
            Assert::IsFalse(figure.canMove(Direction::RIGHT, heap));
        }


        TEST_METHOD(TestCanRotate_BaseRotation)
        {
            vector<vector<bool>> shape = 
            {
                {1, 0},
                {1, 1}
            };

            Point startPos = { 5, 5 };
            vector<vector<bool>> heap(logicalHeight(), vector<bool>(logicalWidth(), false));
            Figure figure(shape, startPos);

            Assert::IsTrue(figure.canRotate(Direction::RIGHT, heap));

            heap[5][6] = true;

            Assert::IsFalse(figure.canRotate(Direction::RIGHT, heap));
        }

        TEST_METHOD(TestCanRotate_RotationBlocked)
        {
            vector<vector<bool>> shape =
            {
                {1, 1},
                {1, 0}
            };
            Point startPos = { 0, 5 };  // Вплотную к левой стене
            vector<vector<bool>> heap(logicalHeight(), vector<bool>(logicalWidth(), false));
            Figure figure(shape, startPos);

            Assert::IsFalse(figure.canRotate(Direction::LEFT, heap), L"Поворот должен быть заблокирован стеной");
        }

        TEST_METHOD(TestCheckPosition)
        {
            vector<vector<bool>> shape = 
            {
                {1, 1},
                {1, 0}
            };
            Point testPos = { 3, 4 };
            vector<vector<bool>> heap(logicalHeight(), vector<bool>(logicalWidth(), false));

            Figure figure(shape, { 5, 5 });

            // Проверяем свободную позицию
            Assert::IsTrue(figure.checkPosition(shape, testPos, heap));

            // Заполняем ячейку в поле и проверяем конфликт
            heap[4][3] = true;
            Assert::IsFalse(figure.checkPosition(shape, testPos, heap));
        }

        TEST_METHOD(TestRotate_Clockwise_and_Counter)
        {
            vector<vector<bool>> shape = 
            {
                {1, 0},
                {1, 1}
            };
            Point startPos = { 5, 5 };

            Figure figure(shape, startPos);

            // Вращаем фигуру по часовой стрелке
            vector<vector<bool>> rotated = figure.rotate(true);
            vector<vector<bool>> expected = 
            {
                {1, 1},
                {1, 0}
            };
            Assert::IsTrue(rotated == expected);

            // Вращаем фигуру против часовой стрелки
            rotated = figure.rotate(false);
            expected = 
            {
                {0, 1},
                {1, 1}
            };
            Assert::IsTrue(rotated == expected);
        }
	};
}
