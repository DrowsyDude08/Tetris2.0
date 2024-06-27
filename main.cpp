#include <SFML/Graphics.hpp>
#include "GLOBAL.hpp"
#include "Tetramino.hpp"

#include <string>
#include <cmath>
#include <random>
#include "Menu.h"

/**
 * @brief Функция рендеринга текста в окне SFML.
 *
 * Эта функция использует заданную текстуру шрифта для рисования текста в окне SFML.
 * Текст рисуется символ за символом, при этом каждый символ уменьшается на 0,5f.
 * Функция поддерживает символы новой строки ('\n') для перемещения текстового курсора на следующую строку.
 *
 * @param i_x X координата левого верхнего угла текста.
 * @param i_y Y координата левого верхнего угла текста.
 * @param i_text Строка, которая будет отображена.
 * @param i_window SFML окно на котором текст будет отображаться.
 */
void RenderText(unsigned short i_x, unsigned short i_y, const std::string &i_text, sf::RenderWindow &i_window)
{
    // Выровняем текст по верхнему левому краю
    short character_x = i_x;
    short character_y = i_y;

    unsigned char character_width;

    sf::Sprite character_sprite;

    sf::Texture font_texture;
    font_texture.loadFromFile("C:/Users/badma/CLionProjects/sfml-try/Font.png");

    // Рассчитаем ширину символа, основываясь на размере изображения шрифта
    // 96, потому что в изображении 96 символов
    character_width = font_texture.getSize().x / 96;

    character_sprite.setTexture(font_texture);

    for (const char a : i_text)
    {
        if ('\n' == a)
        {
            // После каждой новой строки увеличиваем координату y и сбрасываем координату x.
            character_x = i_x;
            character_y += font_texture.getSize().y;

            continue;
        }

        // Изменить положение следующего символа
        character_sprite.setPosition(character_x, character_y);
        // Выберем символ из изображения-шрифта
        character_sprite.setTextureRect(sf::IntRect(character_width * (a - 32), 0, character_width, font_texture.getSize().y));
        character_sprite.setScale(sf::Vector2f(0.5f, 0.5f));

        // Увеличиваем x-координату
        character_x += character_width * .5f;

        // Рисуем символ
        i_window.draw(character_sprite);
    }
}

/**
 * @brief Основная функция игры в Тетрис.
 *
 * Эта функция инициализирует игровое окно, обрабатывает вводимые пользователем данные, обновляет состояние игры и визуализирует игру.
 * Он использует генератор случайных чисел для генерации случайных фигур из тетромино и двумерный массив для представления игрового поля.
 * Цикл игры продолжается до тех пор, пока окно не закроется.
 *
 * @return 0, если игра завершается успешно, ненулевое значение в противном случае.
 */
int main()
{
    // Убедимся, что игра работает независимо от частоты кадров.
    unsigned int timeElapsed = 0;
    sf::Clock clock;

    // Рандомайзер чисел:
    std::random_device rnd_dev;
    std::mt19937 rng(rnd_dev());
    std::uniform_int_distribution<std::mt19937::result_type> gen_random_shape(1, 7);

    // Инициализация окна.
    sf::RenderWindow window(sf::VideoMode(2 * CELL_SIZE * COLS * SCREEN_RESIZE, CELL_SIZE * ROWS * SCREEN_RESIZE), "Tetris", sf::Style::Close);
    window.setView(sf::View(sf::FloatRect(0, 0, 2 * CELL_SIZE * COLS, CELL_SIZE * ROWS)));
    window.setFramerateLimit(60);

    // Обработка событий
    sf::Event event;

    //Tetramino в игре
    Tetramino currTetramino(gen_random_shape(rng));

    // Предпросмотр следующей фигуры
    unsigned char nextShape = gen_random_shape(rng);
    // Счетчик
    unsigned long long score = 0;

    // Окно предпросмотра
    sf::RectangleShape nextPiecePreview(sf::Vector2f(5 * CELL_SIZE, 5 * CELL_SIZE));
    nextPiecePreview.setFillColor(sf::Color(0, 0, 0));
    nextPiecePreview.setOutlineThickness(-1);
    nextPiecePreview.setPosition(CELL_SIZE * (1.5f * COLS - 2.5f), CELL_SIZE * (0.25f * ROWS - 2.5f));

    // Флаги
    bool pressedLeft = 0;
    bool pressedRight = 0;

    bool gameOver = 0;
    //Зачищенные линии
    unsigned long long linesCleared = 0;

    // Матрица игры:
    char matrix[ROWS][COLS];
    for (size_t y = 0; y < ROWS; y++)
    {
        for (size_t x = 0; x < COLS; x++)
        {
            matrix[y][x] = 0;
        }
    }

    while (window.isOpen())
    {
        timeElapsed = clock.getElapsedTime().asMilliseconds();
        pressedLeft = 0;
        pressedRight = 0;

        // События свитча:
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                {
                    window.close();
                    break;
                }
                case sf::Event::KeyPressed:
                {
                    switch (event.key.code)
                    {
                        case sf::Keyboard::Escape:
                        {
                            window.close();
                            break;
                        }
                        case sf::Keyboard::Left:
                        {
                            pressedLeft = 1;
                            break;
                        }
                        case sf::Keyboard::Right:
                        {
                            pressedRight = 1;
                            break;
                        }
                        case sf::Keyboard::Down: // Кнопка "Сброса"
                        {
                            if (gameOver)
                                break;

                            while (currTetramino.MoveDown(matrix))
                                ;

                            currTetramino.UpdateMatrix(matrix);
                            currTetramino = Tetramino(nextShape);
                            nextShape = gen_random_shape(rng);
                            currTetramino.UpdateMatrix(matrix);

                            currTetramino.MoveDown(matrix); // Проверка на столкновение
                            if (currTetramino.CollidedWithATetramino)
                                for (sf::Vector2i mino : currTetramino.GetMinos())
                                    if (mino.y == 0)
                                    {
                                        gameOver = 1;
                                        break;
                                    }


                            for (sf::Vector2i &mino : currTetramino.GetMinos())
                                mino.y -= 1;

                            // Проверка полноты линии:
                            unsigned char currentLinesCleared = 0;
                            for (size_t y = 0; y < ROWS; y++)
                            {
                                bool filledLine = true;
                                for (size_t x = 0; x < COLS; x++)
                                    filledLine = filledLine && matrix[y][x] != 0;

                                if (filledLine)
                                {
                                    currentLinesCleared += 1;
                                    // Ситема коллизий проверка.
                                    for (sf::Vector2i &mino : currTetramino.GetMinos())
                                    {
                                        matrix[mino.y][mino.x] = 0;
                                    }

                                    // Move all rows down by one:
                                    for (size_t y1 = y; y1 >= 1; y1--)
                                        for (size_t x1 = 0; x1 < COLS; x1++)
                                            matrix[y1][x1] = matrix[y1 - 1][x1];

                                    // Reset current tetraminos
                                    for (sf::Vector2i &mino : currTetramino.GetMinos())
                                    {
                                        matrix[mino.y][mino.x] = currTetramino.GetShape();
                                    }
                                }
                            }
                            linesCleared += currentLinesCleared;
                            // Система подсчета.
                            switch (currentLinesCleared)
                            {
                                case 1:
                                    score += 40;
                                    break;
                                case 2:
                                    score += 100;
                                    break;
                                case 3:
                                    score += 300;
                                    break;
                                case 4:
                                    score += 1200;
                                    break;
                            }

                            break;
                        }
                        case sf::Keyboard::Up:
                        {
                            if (gameOver)
                                break;

                            for (sf::Vector2i &mino : currTetramino.GetMinos())
                            {
                                matrix[mino.y][mino.x] = 0;
                            }
                            currTetramino.Rotate(1, matrix);
                            currTetramino.UpdateMatrix(matrix);
                        }
                        case sf::Keyboard::Space:
                        {
                            if (gameOver)
                            {
                                gameOver = 0;

                                for (size_t y = 0; y < ROWS; y++)
                                {
                                    for (size_t x = 0; x < COLS; x++)
                                    {
                                        matrix[y][x] = 0;
                                    }
                                }

                                currTetramino = Tetramino(gen_random_shape(rng));
                                currTetramino.UpdateMatrix(matrix);
                                score = 0;
                                linesCleared = 0;
                            }
                        }
                    }
                    break;
                }
            }
        }

        // Отоброжание экрана смерти:
        if (gameOver)
        {
            window.clear(sf::Color::Black);

            RenderText(
                    static_cast<unsigned short>(0),
                    static_cast<unsigned short>(0),
                    "Game over.\nPress [SPACE] to restart the game.\nPress [Escape] to exit the game. \nScore: " + std::to_string(score),
                    window);

            window.display();
        }
        else
        {
            if ((pressedRight || pressedLeft))
            {
                currTetramino.MoveSide(matrix, pressedRight - pressedLeft);
                currTetramino.UpdateMatrix(matrix);
            }

            // Запустите игровую рамку:
            while (timeElapsed >= FRAME_DURATION_MS)
            {
                timeElapsed -= FRAME_DURATION_MS;
                clock.restart();

                currTetramino.MoveDown(matrix);
                currTetramino.UpdateMatrix(matrix);

                if (currTetramino.CollidedWithATetramino)
                {
                    for (sf::Vector2i mino : currTetramino.GetMinos())
                    {
                        if (mino.y == 0)
                            gameOver = 1;
                    }

                    currTetramino = Tetramino(nextShape);
                    nextShape = gen_random_shape(rng);
                    currTetramino.UpdateMatrix(matrix);
                }

                // Проверка заполненности линии:
                for (size_t y = 0; y < ROWS; y++)
                {
                    bool filledLine = true;
                    for (size_t x = 0; x < COLS; x++)
                        filledLine = filledLine && matrix[y][x] != 0;

                    if (filledLine)
                    {
                        for (sf::Vector2i &mino : currTetramino.GetMinos())
                        {
                            matrix[mino.y][mino.x] = 0;
                        }

                        // Переместим все строки вниз на одну:
                        for (size_t y1 = y; y1 >= 1; y1--)
                            for (size_t x1 = 0; x1 < COLS; x1++)
                                matrix[y1][x1] = matrix[y1 - 1][x1];

                        // Сброс текущего тетрамино
                        for (sf::Vector2i &mino : currTetramino.GetMinos())
                        {
                            matrix[mino.y][mino.x] = currTetramino.GetShape();
                        }
                    }
                }
            }

            // Вывод:
            window.clear(sf::Color::Black);
            sf::RectangleShape cellShape(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));

            // Вывод заднего фона:
            for (size_t y = 0; y < ROWS; y++)
            {
                for (size_t x = 0; x < COLS; x++)
                {
                    if (matrix[y][x] == 0)
                    {
                        cellShape.setFillColor(sf::Color(42, 45, 46));
                        cellShape.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                        window.draw(cellShape);
                    }
                }
            }

            // Тень:
            cellShape.setFillColor(sf::Color(83, 86, 87));
            for (sf::Vector2i mino : currTetramino.GetDropHighlight(matrix))
            {
                cellShape.setPosition(mino.x * CELL_SIZE, mino.y * CELL_SIZE);
                window.draw(cellShape);
            }

            // Вывод(рендер) блоков:
            for (size_t y = 0; y < ROWS; y++)
            {
                for (size_t x = 0; x < COLS; x++)
                {
                    if (matrix[y][x] != 0)
                    {
                        cellShape.setFillColor(Tetramino::ShapeToColour(matrix[y][x]));
                        cellShape.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                        window.draw(cellShape);
                    }
                }
            }

            // Предварительный просмотр:
            window.draw(nextPiecePreview);

            for (sf::Vector2i mino : Tetramino::GetTetramino(nextShape, static_cast<unsigned char>(1.5f * COLS), static_cast<unsigned char>(0.25f * ROWS)))
            {
                // Смещение тетромино в центр границы предварительного просмотра
                unsigned short nextTetraminoX = CELL_SIZE * mino.x;
                unsigned short nextTetraminoY = CELL_SIZE * mino.y;

                if (nextShape == 1)
                {
                    nextTetraminoY += static_cast<unsigned char>(round(0.5f * CELL_SIZE));
                }
                else if (nextShape != 4)
                {
                    nextTetraminoX -= static_cast<unsigned char>(round(0.5f * CELL_SIZE));
                }

                cellShape.setPosition(nextTetraminoX, nextTetraminoY);
                cellShape.setFillColor(Tetramino::ShapeToColour(nextShape));
                window.draw(cellShape);
            }

            // вывод рекорда:
            RenderText(
                    static_cast<unsigned short>(CELL_SIZE * (0.5f + COLS)),
                    static_cast<unsigned short>(0.5f * CELL_SIZE * ROWS),
                    "Lines: " + std::to_string(linesCleared) + "\nScore: " + std::to_string(score),
                    window);

            window.display();
        }
    }

    return 0;
}