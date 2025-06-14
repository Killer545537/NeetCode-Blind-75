#include <bits/stdc++.h>
#include <catch2/catch_test_macros.hpp>

void rotate(std::vector<std::vector<int>>& matrix) {
    const size_t N = matrix.size();

    for (size_t i = 0; i < N; ++i)
        for (size_t j = i + 1; j < N; ++j)
            std::swap(matrix[i][j], matrix[j][i]);

    for (std::vector<int>& row : matrix)
        std::ranges::reverse(row);
}

TEST_CASE("Rotate Image") {
    SECTION("Case 1") {
        std::vector<std::vector<int>> matrix = {
            {1, 2, 3}, {4, 5, 6}, {7, 8, 9}
        };
        rotate(matrix);
        REQUIRE(matrix == std::vector<std::vector<int>>{
                {7, 4, 1}, {8, 5, 2}, {9, 6, 3}
                });
    }

    SECTION("Case 2") {
        std::vector<std::vector<int>> matrix = {
            {5, 1, 9, 11},
            {2, 4, 8, 10},
            {13, 3, 6, 7},
            {15, 14, 12, 16}
        };
        rotate(matrix);
        REQUIRE(matrix == std::vector<std::vector<int>>{
                {15, 13, 2, 5},
                {14, 3, 4, 1},
                {12, 6, 8, 9},
                {16, 7, 10, 11}
                });
    }
}

std::vector<int> spiralOrder(const std::vector<std::vector<int>>& matrix) {
    const size_t ROWS = matrix.size(), COLS = matrix[0].size();
    std::vector<int> spiral;

    int left = 0, right = static_cast<int>(COLS) - 1, top = 0, bottom = static_cast<int>(ROWS) - 1;
    while (left <= right and top <= bottom) {
        // Left -> Right
        for (int j = left; j <= right; ++j) spiral.push_back(matrix[top][j]);
        top++;

        // Top -> Bottom
        for (int i = top; i <= bottom; ++i) spiral.push_back(matrix[i][right]);
        right--;

        // Right -> Left
        if (top <= bottom) {
            for (int j = right; j >= left; --j) spiral.push_back(matrix[bottom][j]);
            bottom--;
        }

        // Bottom -> Top
        if (left <= right) {
            for (int i = bottom; i >= top; --i) spiral.push_back(matrix[i][left]);
            left++;
        }
    }

    return spiral;
}

TEST_CASE("Spiral Matrix") {
    REQUIRE(spiralOrder({
                {1, 2, 3},
                {4, 5, 6},
                {7, 8, 9}
                }) == std::vector<int>{1, 2, 3, 6, 9, 8, 7, 4, 5});
    REQUIRE(spiralOrder({
                {1, 2, 3, 4},
                {5, 6, 7, 8},
                {9, 10, 11, 12}
                }) == std::vector<int>{1, 2, 3, 4, 8, 12, 11, 10, 9, 5, 6, 7});
}

void setZeroes(std::vector<std::vector<int>>& matrix) {
    const size_t ROWS = matrix.size(), COLS = matrix[0].size();

    bool row_zero = false;
    for (size_t row = 0; row < ROWS; ++row)
        for (size_t col = 0; col < COLS; ++col)
            if (matrix[row][col] == 0) {
                matrix[0][col] = 0;

                if (row == 0) row_zero = true;
                else matrix[row][0] = 0;
            }

    for (size_t row = 1; row < ROWS; ++row)
        for (size_t col = 1; col < COLS; ++col)
            if (matrix[0][col] == 0 or matrix[row][0] == 0) matrix[row][col] = 0;

    if (matrix[0][0] == 0)
        for (size_t row = 0; row < ROWS; ++row) matrix[row][0] = 0;

    if (row_zero)
        for (size_t col = 0; col < COLS; ++col) matrix[0][col] = 0;
}

TEST_CASE("Set Matrix Zeroes") {
    SECTION("Case 1") {
        std::vector<std::vector<int>> matrix = {
            {1, 1, 1},
            {1, 0, 1},
            {1, 1, 1}
        };
        setZeroes(matrix);
        REQUIRE(matrix == std::vector<std::vector<int>>{
                {1, 0, 1},
                {0, 0, 0},
                {1, 0, 1}
                });
    }
    SECTION("Case 2") {
        std::vector<std::vector<int>> matrix = {
            {0, 1, 2, 0},
            {3, 4, 5, 2},
            {1, 3, 1, 5}
        };
        setZeroes(matrix);
        REQUIRE(matrix == std::vector<std::vector<int>>{
                {0, 0, 0, 0},
                {0, 4, 5, 0},
                {0, 3, 1, 0}
                });
    }
}
