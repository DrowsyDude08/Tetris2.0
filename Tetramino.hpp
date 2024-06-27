#pragma once

#include <vector>
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Color.hpp"
#include "GLOBAL.hpp"
#include <iostream>

/**
 * @brief, Класс, представляющий Tetris tetromino.
 *
 * Этот класс отвечает за управление Tetris tetromino, включая его форму, положение,
перемещение, обнаружение столкновений и вращение.
 */
class Tetramino
{
private:
    unsigned char shape;
    unsigned char rotation;
    sf::Vector2i pos;
    std::vector<sf::Vector2i> minos;

public:
    // Указывает, столкнулся ли тетрамино с другим тетрамино или с полом.
    bool CollidedWithATetramino = 0;

    // Конструктор для инициализации тетрамино заданной формы.
    Tetramino(unsigned char p_shape);

    // Движение вниз. TRUE -> моежт двигаться; FALSE -> не может
    template <size_t rows, size_t cols>
    bool MoveDown(char (&p_matrix)[rows][cols])
    {
        for (sf::Vector2i &mino : minos)
        {
            p_matrix[mino.y][mino.x] = 0;
        }

        // Check collisions
        for (sf::Vector2i &mino : minos)
        {
            // One ahead as we are "predicting" movement
            if (mino.y + 1 == ROWS)
            {
                CollidedWithATetramino = 1;

                // Reset minos
                for (sf::Vector2i &mino : minos)
                {
                    p_matrix[mino.y][mino.x] = shape;
                }

                return 0;
            }

            if (p_matrix[mino.y + 1][mino.x] != 0)
            {
                CollidedWithATetramino = 1;

                // Reset minos
                for (sf::Vector2i &mino : minos)
                {
                    p_matrix[mino.y][mino.x] = shape;
                }

                return 0;
            }
        }

        for (sf::Vector2i &mino : minos)
            mino.y += 1;

        pos.y += 1;

        return 1;
    }

    // Движение вбок. TRUE -> моежт двигаться; FALSE -> не может
    template <size_t rows, size_t cols>
    bool MoveSide(char (&p_matrix)[rows][cols], short p_mod)
    {

        for (sf::Vector2i &mino : minos)
        {
            p_matrix[mino.y][mino.x] = 0;
        }

        // Проверка коллизий:
        for (sf::Vector2i &mino : minos)
        {
            if (mino.x + p_mod == COLS || mino.x + p_mod <= -1)
            {

                // Сброс
                for (sf::Vector2i &mino : minos)
                {
                    p_matrix[mino.y][mino.x] = shape;
                }

                return 0;
            }

            if (p_matrix[mino.y][mino.x + p_mod])
            {
                CollidedWithATetramino = 1;

                // Сброс
                for (sf::Vector2i &mino : minos)
                {
                    p_matrix[mino.y][mino.x] = shape;
                }

                return 0;
            }
        }

        for (sf::Vector2i &mino : minos)
            mino.x += p_mod;

        pos.x += p_mod;

        return 1;
    }

    // Вычислите позиции блоков, которые были бы заняты, если бы тетрамино был отброшен.
    template <size_t rows, size_t cols>
    std::vector<sf::Vector2i> GetDropHighlight(char (&p_matrix)[rows][cols])
    {
        short blocksFallen = 0;
        bool collided = 0;
        bool collidedWithSelf = 0;

        for (sf::Vector2i &mino : minos)
        {
            p_matrix[mino.y][mino.x] = 0;
        }

        while (!collided)
        {
            for (auto mino : minos)
            {
                if (p_matrix[mino.y + blocksFallen][mino.x] != 0 || mino.y + blocksFallen == 20)
                {
                    collided = 1;
                    break;
                }
            }
            blocksFallen += 1;
        }

        // Сброс
        for (sf::Vector2i &mino : minos)
        {
            p_matrix[mino.y][mino.x] = shape;
        }

        std::vector<sf::Vector2i> ghostMinos(4);
        for (size_t i = 0; i < 4; i++)
        {
            ghostMinos[i].y = minos[i].y + blocksFallen - 2;
            ghostMinos[i].x = minos[i].x;
        }

        return ghostMinos;
    }

    void Reset(unsigned char p_shape);

    // Courtesy of KofyBrek(https://www.youtube.com/c/Kofybrek)
/**
 * @brief Позволяет получить данные об ударе о стену для заданной формы и поворота.
 *
* @param p_isShape Указывает, является ли фигура кубом или нет.
 * @param p_currRotation - текущее вращение фигуры.
 * @param p_nextRotation Следующий поворот фигуры.
 * @param std::vector<sf::Vector2i> Набор двумерных векторов, представляющих данные об ударе о стену.
 */
    std::vector<sf::Vector2i> GetWallKickData(bool p_isShape, unsigned char p_currRotation, unsigned char p_nextRotation)
    {
        if (0 == p_isShape)
        {
            switch (p_currRotation)
            {
                case 0:
                case 2:
                {
                    switch (p_nextRotation)
                    {
                        case 1:
                        {
                            return {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}};
                        }
                        case 3:
                        {
                            return {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}};
                        }
                    }
                }
                case 1:
                {
                    return {{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}};
                }
                case 3:
                {
                    return {{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}};
                }
            }

            return {{0, 0}};
        }
        else
        {
            switch (p_currRotation)
            {
                case 0:
                {
                    switch (p_nextRotation)
                    {
                        case 1:
                        {
                            return {{0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -2}};
                        }
                        case 3:
                        {
                            return {{0, 0}, {-1, 0}, {2, 0}, {-1, -2}, {2, 1}};
                        }
                    }
                }
                case 1:
                {
                    switch (p_nextRotation)
                    {
                        case 0:
                        {
                            return {{0, 0}, {2, 0}, {-1, 0}, {2, -1}, {-1, 2}};
                        }
                        case 2:
                        {
                            return {{0, 0}, {-1, 0}, {2, 0}, {-1, -2}, {2, 1}};
                        }
                    }
                }
                case 2:
                {
                    switch (p_nextRotation)
                    {
                        case 1:
                        {
                            return {{0, 0}, {1, 0}, {-2, 0}, {1, 2}, {-2, -1}};
                        }
                        case 3:
                        {
                            return {{0, 0}, {2, 0}, {-1, 0}, {2, -1}, {-1, 2}};
                        }
                    }
                }
                case 3:
                {
                    switch (p_nextRotation)
                    {
                        case 0:
                        {
                            return {{0, 0}, {1, 0}, {-2, 0}, {1, 2}, {-2, -1}};
                        }
                        case 2:
                        {
                            return {{0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -2}};
                        }
                    }
                }
            }

            return {{0, 0}};
        }
    }

    // Courtesy of KofyBrek(https://www.youtube.com/c/Kofybrek)
    template <size_t rows, size_t cols>
    void Rotate(bool p_clockwise, const char (&p_matrix)[rows][cols])
    {
        // If not cube
        if (shape != 4)
        {
            std::vector<sf::Vector2i> currentMinos = minos;
            unsigned char nextRotation = ((p_clockwise ? 1 : 3) + rotation) % 4;

            if (shape == 1)
            {
                float centreX = 0.5f * (minos[1].x + minos[2].x);
                float centreY = 0.5f * (minos[1].y + minos[2].y);

                if (rotation == 0)
                    centreY += 0.5f;
                else if (rotation == 1)
                    centreX -= 0.5f;
                else if (rotation == 2)
                    centreY -= 0.5f;
                else if (rotation == 3)
                    centreX += 0.5f;

                for (sf::Vector2i &mino : minos)
                {
                    float x = mino.x - centreX;
                    float y = mino.y - centreY;

                    if (p_clockwise)
                    {
                        mino.x = static_cast<char>(centreX - y);
                        mino.y = static_cast<char>(centreY + x);
                    }
                    else
                    {
                        mino.x = static_cast<char>(centreX + y);
                        mino.y = static_cast<char>(centreY - x);
                    }
                }
            }
            else
            {
                for (unsigned char a = 1; a < minos.size(); a++)
                {
                    char x = minos[a].x - minos[0].x;
                    char y = minos[a].y - minos[0].y;

                    if (p_clockwise)
                    {
                        minos[a].x = minos[0].x - y;
                        minos[a].y = x + minos[0].y;
                    }
                    else
                    {
                        minos[a].x = y + minos[0].x;
                        minos[a].y = minos[0].y - x;
                    }
                }
            }

            for (sf::Vector2i &wallKick : GetWallKickData(shape == 1, rotation, nextRotation))
            {
                bool canTrun = 1;

                for (sf::Vector2i &mino : minos)
                {
                    if (mino.x + wallKick.x < 0 || COLS <= mino.x + wallKick.x || ROWS <= mino.y + wallKick.y)
                    {
                        canTrun = 0;
                        break;
                    }

                    if (mino.y + wallKick.y < 0)
                    {
                        continue;
                    }
                    else if (p_matrix[mino.x + wallKick.x][mino.y + wallKick.y])
                    {
                        canTrun = 0;
                        break;
                    }
                }

                if (canTrun)
                {
                    rotation = nextRotation;

                    for (sf::Vector2i &mino : minos)
                    {
                        mino.x += wallKick.x;
                        mino.y += wallKick.y;
                    }

                    return;
                }
            }

            minos.assign(currentMinos.begin(), currentMinos.end());
        }
    }

    /**
 * @brief Обновляет игровую матрицу с учетом позиции текущего тетрамино.
 *
 * Эта функция выполняет итерацию по минонам текущего тетрамино и обновляет игровую матрицу с учетом формы тетрамино в их соответствующих позициях.
 *
 * @tparam rows  отображает количество строк в игровой матрице.
 * @tparam cols отображает количество столбцов в игровой матрице.
 * @param p_matrix - Игровая матрица, которую необходимо обновить.
 */
    template <size_t rows, size_t cols>
    void UpdateMatrix(char (&p_matrix)[rows][cols])
    {
        for (sf::Vector2i &mino : minos)
        {
            p_matrix[mino.y][mino.x] = shape;
        }
    }

    static std::vector<sf::Vector2i> GetTetramino(unsigned char p_shape, unsigned char p_x, unsigned char p_y);

    /**
 * @brief Функция преобразует номер фигуры в соответствующий ей цвет.
 *
 * Эта функция принимает номер фигуры в качестве входных данных и возвращает соответствующий цвет.
 * Номера фигур определены в классе Tetramino и соответствуют конкретным фигурам.
 *
 * @param p_shape - преобразовать номер фигуры в цвет.
 * @return цвет, соответствующий заданному номеру фигуры.
 * Если номер фигуры не распознан, возвращается sf::Color::Black.
 */
    static sf::Color ShapeToColour(unsigned char p_shape)
    {
        switch (p_shape)
        {
            case 1:
                return sf::Color(66, 135, 245); // Blue
            case 2:
                return sf::Color(61, 237, 12); // Green
            case 3:
                return sf::Color(27, 12, 237); // Dark Blue
            case 4:
                return sf::Color(237, 230, 12); // Yellow
            case 5:
                return sf::Color(237, 12, 12); // Red
            case 6:
                return sf::Color(230, 16, 215); // Purple
            case 7:
                return sf::Color(230, 148, 16); // Orange
        }

        return sf::Color::Black;// Цвет по умолчанию, если номер фигуры не распознан
    }

    /**
 * @brief Возвращает форму текущего тетрамино.
 *
 * Эта функция возвращает форму текущего тетрамино.
 * Форма представлена символом без знака, где каждое значение соответствует определенной форме тетрамино.
 *
 * @return символ без знака - форму текущего тетрамино.
 *
 * @note. Значения формы определены в классе Tetramino и соответствуют конкретным тетрамино.
 * @note: если фигура не распознана, функция вернет значение 0 (sf::Color::Black).
 */
    unsigned char GetShape()
    {
        return shape;
    }

    /**
 * @brief Возвращает ссылку на вектор точек, составляющих текущее тетрамино.
 *
 * Эта функция возвращает ссылку на вектор точек, составляющих текущее тетрамино.
 * Каждый mino представлен двумерным вектором целых чисел, где первым элементом является координата x, а вторым элементом - координата y.
 *
 * @return ссылку на вектор mino.
 *
 * @note вектор minos обновляется каждый раз, когда Tetramino перемещается, вращается или сбрасывается.
 * @note возвращаемый вектор не следует изменять напрямую, так как это может повлиять на внутреннее состояние объекта Tetramino.
 */
    std::vector<sf::Vector2i> &GetMinos()
    {
        return minos;
    }
};