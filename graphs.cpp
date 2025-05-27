#include <bits/stdc++.h>
#include <catch2/catch_template_test_macros.hpp>

int numIslands(const std::vector<std::vector<char>>& grid) {
    if (grid.empty() or grid[0].empty()) return 0;
    const size_t ROWS = grid.size(), COLS = grid[0].size();
    std::vector visited(ROWS, std::vector(COLS, false));
    constexpr std::array<std::pair<int, int>, 4> DIRECTIONS{
        {
            {-1, 0}, {1, 0}, {0, 1}, {0, -1}
        }
    };

    const auto is_valid = [&](const int row, const int col) {
        return row >= 0 and row < ROWS
            and col >= 0 and col < COLS
            and grid[row][col] == '1' and !visited[row][col];
    };
    const auto bfs = [&](const int row, const int col) {
        std::queue<std::pair<int, int>> queue;
        queue.emplace(row, col);
        visited[row][col] = true;

        while (!queue.empty()) {
            const auto [current_row, current_col] = queue.front();
            queue.pop();

            for (const auto& [dr, dc] : DIRECTIONS) {
                if (const int new_row = current_row + dr, new_col = current_col + dc;
                    is_valid(new_row, new_col)) {
                    visited[new_row][new_col] = true;
                    queue.emplace(new_row, new_col);
                }
            }
        }
    };

    int islands = 0;
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < COLS; ++j) {
            if (grid[i][j] == '1' and !visited[i][j]) {
                bfs(i, j);
                islands++;
            }
        }

    return islands;
}

TEST_CASE("Number of Islands") {
    SECTION("Test 1") {
        const std::vector<std::vector<char>> grid = {
            {'1', '1', '1', '1', '0'},
            {'1', '1', '0', '1', '0'},
            {'1', '1', '0', '0', '0'},
            {'0', '0', '0', '0', '0'}
        };
        REQUIRE(numIslands(grid) == 1);
    }
    SECTION("Test 2") {
        const std::vector<std::vector<char>> grid = {
            {'1', '1', '0', '0', '0'},
            {'1', '1', '0', '0', '0'},
            {'0', '0', '1', '0', '0'},
            {'0', '0', '0', '1', '1'}
        };
        REQUIRE(numIslands(grid) == 3);
    }
}

class Node {
public:
    int val;
    std::vector<Node*> neighbors;

    Node() {
        val = 0;
        neighbors = std::vector<Node*>();
    }

    explicit Node(const int _val) {
        val = _val;
        neighbors = std::vector<Node*>();
    }

    Node(const int _val, const std::vector<Node*>& _neighbours) {
        val = _val;
        neighbors = _neighbours;
    }
};

Node* cloneGraph(Node* node) {
    if (!node) return nullptr;
    std::unordered_map<Node*, Node*> clones;

    std::function<Node*(Node*)> dfs = [&](Node* curr) -> Node* {
        if (clones.contains(curr)) return clones[curr];
        // Create the clone
        auto* copy = new Node(curr->val);
        clones[curr] = copy;
        // Push the clone of the neighbors
        for (Node* neighbour : curr->neighbors) copy->neighbors.push_back(dfs(neighbour));

        return copy;
    };

    return dfs(node);
}

/// The Pacific Ocean touches the top and left.
/// The Atlantic Ocean touches the bottom and right.
std::vector<std::vector<int>> pacificAtlantic(const std::vector<std::vector<int>>& heights) {
    // Start DFS/BFS from the Pacific and Atlantic Oceans and see if a cell can be reached
    const size_t ROWS = heights.size(), COLS = heights[0].size();
    constexpr std::array<std::pair<int, int>, 4> DIRECTIONS{
        {{-1, 0}, {1, 0}, {0, 1}, {0, -1}}
    };
    std::vector reachable_pacific(ROWS, std::vector(COLS, false));
    std::vector reachable_atlantic(ROWS, std::vector(COLS, false));

    const auto is_inside = [ROWS, COLS](const int row, const int col, const std::vector<std::vector<bool>>& ocean) {
        return row >= 0 and row < ROWS
            and col >= 0 and col < COLS
            and !ocean[row][col];
    };
    const std::function<void(int, int, std::vector<std::vector<bool>>&)> dfs =
        [&](const int row, const int col,
            std::vector<std::vector<bool>>& ocean) {
        ocean[row][col] = true;

        for (const auto& [dr, dc] : DIRECTIONS) {
            if (const int new_row = row + dr, new_col = col + dc
                ; is_inside(new_row, new_col, ocean) and heights[new_row][new_col] >= heights[row][col])
                dfs(new_row, new_col, ocean);
        }
    };

    for (int col = 0; col < COLS; ++col) dfs(0, col, reachable_pacific);
    for (int row = 0; row < ROWS; ++row) dfs(row, 0, reachable_pacific);

    for (int col = 0; col < COLS; ++col) dfs(ROWS - 1, col, reachable_atlantic);
    for (int row = 0; row < ROWS; ++row) dfs(row, COLS - 1, reachable_atlantic);

    std::vector<std::vector<int>> result;
    for (int row = 0; row < ROWS; ++row)
        for (int col = 0; col < COLS; ++col)
            if (reachable_pacific[row][col] and reachable_atlantic[row][col]) result.push_back({row, col});

    return result;
}

TEST_CASE("Pacific Atlantic Water Flow") {
    auto sort_results = [](const std::vector<std::vector<int>>& vector) {
        auto vec = vector;
        std::ranges::sort(vec);
        return vec;
    };

    REQUIRE(sort_results(pacificAtlantic( {
        {1,2,2,3,5},
        {3,2,3,4,4},
        {2,4,5,3,1},
        {6,7,1,4,5},
        {5,1,1,2,4}
        })) == sort_results( {
        {0,4},{1,3},{1,4},{2,2},{3,0},{3,1},{4,0}
        }));

    REQUIRE(sort_results(pacificAtlantic({{1}})) == sort_results({{0, 0}}));
}

/// Can do it using Kahn's Algorithm and checking if `topo.len === numCourses`
bool canFinish(const int numCourses, const std::vector<std::vector<int>>& prerequisites) {
    const auto make_adj_list = [numCourses](const std::vector<std::vector<int>>& edges) {
        std::vector<std::vector<int>> adj_list(numCourses);
        for (const auto& edge : edges) {
            const auto u = edge[0], v = edge[1];
            adj_list[u].push_back(v);
        };
        return adj_list;
    };
    const auto get_indegree = [](const std::vector<std::vector<int>>& adj_list) {
        std::vector indegree(adj_list.size(), 0);
        for (const auto& u : adj_list) for (const int v : u) indegree[v]++;
        return indegree;
    };

    const auto adj_list = make_adj_list(prerequisites);
    auto indegree = get_indegree(adj_list);

    std::queue<int> queue;
    for (int i = 0; i < numCourses; ++i) if (indegree[i] == 0) queue.push(i);

    int can_finish = 0;
    while (!queue.empty()) {
        const int course = queue.front();
        queue.pop();

        can_finish++;
        for (const int next_course : adj_list[course])
            if (--indegree[next_course] == 0) queue.push(next_course);
    }

    return can_finish == numCourses;
}

TEST_CASE("Course Schedule") {
    REQUIRE(canFinish(2, {{1, 0}}) == true);
    REQUIRE(canFinish(2, {{1, 0}, {0, 1}}) == false);
}

class DSU {
    std::vector<int> parent, size;
};

bool validTree(const int n, const std::vector<std::vector<int>>& edges) {

}