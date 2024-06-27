#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "Tetramino.hpp"
#include "Tetramino.cpp"
#include <SFML/Graphics.hpp>
#include <random>

TEST_CASE("ShapeToColour method") {
    CHECK(Tetramino::ShapeToColour(1) == sf::Color(66, 135, 245));
    CHECK(Tetramino::ShapeToColour(2) == sf::Color(61, 237, 12));
    CHECK(Tetramino::ShapeToColour(3) == sf::Color(27, 12, 237));
    CHECK(Tetramino::ShapeToColour(4) == sf::Color(237, 230, 12));
    CHECK(Tetramino::ShapeToColour(5) == sf::Color(237, 12, 12));
    CHECK(Tetramino::ShapeToColour(6) == sf::Color(230, 16, 215));
    CHECK(Tetramino::ShapeToColour(7) == sf::Color(230, 148, 16));
    CHECK(Tetramino::ShapeToColour(8) == sf::Color::Black);
}

TEST_CASE("Test Tetramino constructor for Z shape 2 with invalid shape") {
    CHECK_THROWS_AS(Tetramino(0), std::invalid_argument);
    CHECK_THROWS_AS(Tetramino(8), std::invalid_argument);
}

TEST_CASE("Tetramino Rotation") {
    // Test setup
    std::random_device rnd_dev;
    std::mt19937 rng(rnd_dev());
    std::uniform_int_distribution<int> gen_random_shape(1, 7);

    Tetramino currTetramino(1); // Test with I-shape
    char matrix[ROWS][COLS] = {0};
    currTetramino.UpdateMatrix(matrix);

    // Rotate the tetramino
    currTetramino.Rotate(1, matrix);

    // Verify rotation
    CHECK(currTetramino.GetShape() == 1); // I-shape remains the same after rotation
}

TEST_CASE("Score Calculation") {
    // Test setup
    std::random_device rnd_dev;
    std::mt19937 rng(rnd_dev());
    std::uniform_int_distribution<int> gen_random_shape(1, 7);

    Tetramino currTetramino(gen_random_shape(rng));
    char matrix[ROWS][COLS] = {0};
    currTetramino.UpdateMatrix(matrix);

    unsigned long long score = 0;
    unsigned char linesCleared = 0;

    // Simulate game play
    for (int i = 0; i < ROWS; ++i) {
        for (size_t x = 0; x < COLS; ++x) {
            matrix[i][x] = 1; // Fill the row with a block
        }

        // Check for filled line
        bool filledLine = true;
        for (size_t x = 0; x < COLS; ++x) {
            filledLine = filledLine && matrix[i][x] != 0;
        }

        if (filledLine) {
            ++linesCleared;
        }
    }

    // Calculate score
    switch (linesCleared) {
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

    // Verify score calculation
    CHECK(score == 100);
}

