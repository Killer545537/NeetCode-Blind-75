#include <bits/stdc++.h>
#include <catch2/catch_template_test_macros.hpp>

std::vector<std::vector<int>> combinationSum(const std::vector<int>& candidates, const int target) {
    std::vector<std::vector<int>> combinations;
    std::vector<int> combination;

    const std::function<void(size_t, int)> helper = [&](const size_t ind, const int curr_target) {
        if (ind == candidates.size()) {
            if (curr_target == 0) combinations.emplace_back(combination);
            return;
        }

        if (candidates[ind] <= curr_target) {
            combination.push_back(candidates[ind]);
            helper(ind, curr_target - candidates[ind]);
            combination.pop_back();
        }

        helper(ind + 1, curr_target);
    };

    helper(0, target);

    return combinations;
}

TEST_CASE("Combination Sum") {
    auto sortResults = [](std::vector<std::vector<int>> results) {
        for (auto& result : results) {
            std::ranges::sort(result);
        }
        std::ranges::sort(results);
        return results;
    };

    REQUIRE(sortResults(combinationSum({2, 3, 6, 7}, 7)) ==
        sortResults(std::vector<std::vector<int>>{{2, 2, 3}, {7}}));

    REQUIRE(sortResults(combinationSum({2, 3, 5}, 8)) ==
        sortResults(std::vector<std::vector<int>>{{2, 2, 2, 2}, {2, 3, 3}, {3, 5}}));

    REQUIRE(combinationSum({2}, 1).empty());
}

bool exist(const std::vector<std::vector<char>>& board, const std::string& word) {
    const size_t ROWS = board.size(), COLS = board[0].size();
    const std::vector<std::pair<int, int>> DIRECTIONS{{{0, 1}, {0, -1}, {1, 0}, {-1, 0}}};
    std::vector visited(ROWS, std::vector(COLS, false));

    const std::function<bool(size_t, int, int)> helper = [&](const size_t ind, const int row, const int col) {
        if (ind == word.size()) return true;

        if (row < 0 or row >= ROWS or col < 0 or col >= COLS or board[row][col] != word[ind] or visited[row][col])
            return false;

        visited[row][col] = true;
        for (const auto& [dx, dy] : DIRECTIONS)
            if (helper(ind + 1, row + dx, col + dy)) {
                visited[row][col] = false;
                return true;
            }

        visited[row][col] = false;
        return false;
    };

    for (size_t row = 0; row < ROWS; ++row)
        for (size_t col = 0; col < COLS; ++col)
            if (helper(0, row, col)) return true;

    return false;
}

TEST_CASE("Word Search") {
    const std::vector<std::vector<char>> board = {
        {'A', 'B', 'C', 'E'},
        {'S', 'F', 'C', 'S'},
        {'A', 'D', 'E', 'E'}
    };

    REQUIRE(exist(board, "ABCCED") == true);

    REQUIRE(exist(board, "SEE") == true);

    REQUIRE(exist(board, "ABCB") == false);
}
