#include <bits/stdc++.h>
#include <catch2/catch_template_test_macros.hpp>

namespace implementation {
    class Trie {
        struct TrieNode {
            std::unordered_map<char, TrieNode*> children;
            bool is_end_of_word{false};
        };

        TrieNode* root;

        [[nodiscard]] TrieNode* findNode(const std::string& str) const {
            TrieNode* current = root;

            for (const char ch : str) {
                if (!current->children.contains(ch)) return nullptr;
                current = current->children[ch];
            }

            return current;
        }

        void deleteTrie(TrieNode* node) {
            if (!node) return;

            for (auto& child : node->children | std::views::values) deleteTrie(child);

            delete node;
        }

    public:
        Trie() {
            root = new TrieNode();
        }

        ~Trie() {
            deleteTrie(root);
        }

        void insert(const std::string& word) {
            TrieNode* current = root;

            for (const char ch : word) {
                if (!current->children.contains(ch)) current->children[ch] = new TrieNode();
                current = current->children[ch];
            }

            current->is_end_of_word = true;
        }

        [[nodiscard]] bool search(const std::string& word) const {
            const TrieNode* node = findNode(word);

            return node != nullptr and node->is_end_of_word;
        }

        [[nodiscard]] bool startsWith(const std::string& prefix) const {
            return findNode(prefix) != nullptr;
        }
    };

    TEST_CASE("Trie operations") {
        SECTION("Test basic Trie operations") {
            // ["Trie", "insert", "search", "search", "startsWith", "insert", "search"]
            // [[], ["apple"], ["apple"], ["app"], ["app"], ["app"], ["app"]]
            // [null, null, true, false, true, null, true]

            Trie trie;

            trie.insert("apple");
            REQUIRE(trie.search("apple") == true);
            REQUIRE(trie.search("app") == false);
            REQUIRE(trie.startsWith("app") == true);

            trie.insert("app");
            REQUIRE(trie.search("app") == true);
        }

        SECTION("Additional edge cases") {
            Trie trie;

            REQUIRE(trie.search("") == false);
            REQUIRE(trie.startsWith("") == true);

            trie.insert("");
            REQUIRE(trie.search("") == true);

            trie.insert("a");
            REQUIRE(trie.startsWith("a") == true);
            REQUIRE(trie.search("ab") == false);
        }
    }
}

namespace add_and_search {
    class WordDictionary {
        struct TrieNode {
            std::unordered_map<char, TrieNode*> children;
            bool is_end_of_word = false;

            static void deleteTrie(TrieNode* node) {
                if (!node) return;

                for (auto& child : node->children | std::views::values) deleteTrie(child);

                delete node;
            }
        };

        TrieNode* root;

        static bool dfs(const std::string& word, const size_t ind, TrieNode* node) {
            if (ind == word.length()) return node->is_end_of_word;

            if (word[ind] == '.') {
                return std::ranges::any_of(node->children | std::views::values, [&](const auto& child) {
                    return dfs(word, ind + 1, child);
                });
            }
            if (!node->children.contains(word[ind])) return false;

            return dfs(word, ind + 1, node->children[word[ind]]);
        }

    public:
        WordDictionary() : root(new TrieNode) {}

        ~WordDictionary() {
            TrieNode::deleteTrie(root);
        }

        void addWord(const std::string& word) {
            TrieNode* current = root;

            for (const char ch : word) {
                if (!current->children.contains(ch)) current->children[ch] = new TrieNode();
                current = current->children[ch];
            }

            current->is_end_of_word = true;
        }

        bool search(const std::string& word) {
            return dfs(word, 0, root);
        }
    };

    TEST_CASE("WordDictionary operations") {
        SECTION("Test WordDictionary with wildcards") {
            // ["WordDictionary","addWord","addWord","addWord","search","search","search","search"]
            // [[],["bad"],["dad"],["mad"],["pad"],["bad"],[".ad"],["b.."]]
            // [null,null,null,null,false,true,true,true]

            WordDictionary dict;

            dict.addWord("bad");
            dict.addWord("dad");
            dict.addWord("mad");

            REQUIRE(dict.search("pad") == false);
            REQUIRE(dict.search("bad") == true);
            REQUIRE(dict.search(".ad") == true);
            REQUIRE(dict.search("b..") == true);
        }

        SECTION("Additional edge cases") {
            WordDictionary dict;

            REQUIRE(dict.search("") == false);

            dict.addWord("");
            REQUIRE(dict.search("") == true);

            dict.addWord("hello");
            REQUIRE(dict.search("h....") == true);
            REQUIRE(dict.search("h...") == false);
            REQUIRE(dict.search(".....") == true);
        }
    }
}

namespace word_search {
    class Trie {
    public:
        struct TrieNode {
            std::unordered_map<char, TrieNode*> children;
            bool is_end_of_word = false;
        };

        Trie() : root(new TrieNode()) {}

        void insert(const std::string& word) {
            TrieNode* current = root;

            for (const char ch : word) {
                if (!current->children.contains(ch)) current->children[ch] = new TrieNode();
                current = current->children[ch];
            }

            current->is_end_of_word = true;
        }

        TrieNode* getRoot() const { return root; }

    private:
        TrieNode* root;
    };

    std::vector<std::string> findWords(const std::vector<std::vector<char>>& board,
                                       const std::vector<std::string>& words) {
        const size_t ROWS = board.size(), COLS = board[0].size();
        constexpr std::array<std::pair<int, int>, 4> DIRECTIONS{
            {
                {0, 1}, {0, -1}, {1, 0}, {-1, 0}
            }
        };

        auto trie = Trie();
        std::ranges::for_each(words, [&](const std::string& word) { trie.insert(word); });

        std::unordered_set<std::string> found;
        std::vector visited(ROWS, std::vector(COLS, false));

        const auto invalid = [&](const int row, const int col, const Trie::TrieNode* node) {
            return row < 0 or row == ROWS
                or col < 0 or col == COLS
                or visited[row][col] or !node->children.contains(board[row][col]);
        };
        const std::function<void(int, int, std::string, Trie::TrieNode*)> dfs =
            [&](const int row, const int col, const std::string& word, Trie::TrieNode* node) {
            if (invalid(row, col, node)) return;

            visited[row][col] = true;
            node = node->children[board[row][col]];

            if (node->is_end_of_word) found.insert(word + board[row][col]);

            for (const auto& [dx, dy] : DIRECTIONS) dfs(row + dx, col + dy, word + board[row][col], node);

            visited[row][col] = false;
        };

        for (size_t i = 0; i < ROWS; ++i)
            for (size_t j = 0; j < COLS; ++j)
                dfs(i, j, "", trie.getRoot());

        return {found.begin(), found.end()};
    }

    TEST_CASE("Word Search 2") {
        SECTION("Case 1") {
            const std::vector<std::vector<char>> board = {
                {'o', 'a', 'a', 'n'},
                {'e', 't', 'a', 'e'},
                {'i', 'h', 'k', 'r'},
                {'i', 'f', 'l', 'v'}
            };
            std::vector<std::string> words = {"oath", "pea", "eat", "rain"};
            auto result = findWords(board, words);
            std::vector<std::string> expected = {"eat", "oath"};
            std::ranges::sort(result);
            std::ranges::sort(expected);
            REQUIRE(result == expected);
        }

        SECTION("Case 2") {
            const std::vector<std::vector<char>> board = {
                {'a', 'b'},
                {'c', 'd'}
            };
            std::vector<std::string> words = {"abcb"};
            auto result = findWords(board, words);
            REQUIRE(result.empty());
        }
    }
}
