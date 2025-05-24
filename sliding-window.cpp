#include <bits/stdc++.h>
#include <catch2/catch_test_macros.hpp>

int maxProfit(const std::vector<int>& prices) {
    int low = prices[0], max_profit = 0;

    for (size_t i = 1; i < prices.size(); ++i) {
        max_profit = std::max(max_profit, prices[i] - low);
        if (prices[i] < low) low = prices[i];
    }

    return max_profit;
}

TEST_CASE("Best Time to Buy and Sell Stock") {
    REQUIRE(maxProfit({7,1,5,3,6,4}) == 5);
    REQUIRE(maxProfit({7,6,4,3,1}) == 0);
}

int lengthOfLongestSubstring(const std::string& s) {
    std::unordered_map<char, int> last_seen;
    int max_length = 0, left = 0;

    for (int right = 0; right < static_cast<int>(s.length()); ++right) {
        if (last_seen.contains(s[right]) and last_seen[s[right]] >= left) left = last_seen[s[right]] + 1;

        last_seen[s[right]] = right;
        max_length = std::max(max_length, right - left + 1);
    }

    return max_length;
}

TEST_CASE("Longest Substring Without Repeating Characters") {
    REQUIRE(lengthOfLongestSubstring("abcabcbb") == 3);
    REQUIRE(lengthOfLongestSubstring("bbbbb") == 1);
    REQUIRE(lengthOfLongestSubstring("pwwkew") == 3);
}

int characterReplacement(const std::string& s, const int k) {
    std::unordered_map<char, int> freq_map;

    int longest = 0, max_frequency = 0;
    for (size_t right = 0, left = 0; right < s.length(); ++right) {
        freq_map[s[right]]++;
        max_frequency = std::max(max_frequency, freq_map[s[right]]);

        while ((right - left + 1) - max_frequency > k) {
            freq_map[s[left]]--;
            left++;
        }

        longest = std::max(longest, static_cast<int>(right - left + 1));
    }

    return longest;
}

TEST_CASE("Longest Repeating Character Replacement") {
    REQUIRE(characterReplacement("ABAB", 2) == 4);
    REQUIRE(characterReplacement("AABABBA", 1) == 4);
}

std::string minWindow(const std::string& s, const std::string& t) {
    if (t.empty()) return "";

    std::unordered_map<char, int> freq_t, window;
    for (const char c : t) freq_t[c]++;

    int have = 0, need = freq_t.size();
    int left = 0, min_len = INT_MAX, min_start = 0;

    for (int right = 0; right < static_cast<int>(s.length()); ++right) {
        if (freq_t.contains(s[right]) and ++window[s[right]] == freq_t[s[right]])
            ++have;

        while (have == need) {
            if (right - left + 1 < min_len) {
                min_len = right - left + 1;
                min_start = left;
            }

            if (freq_t.contains(s[left]) and --window[s[left]] < freq_t[s[left]])
                --have;

            ++left;
        }
    }

    return min_len == INT_MAX ? "" : s.substr(min_start, min_len);
}

TEST_CASE("Minimum Window Substring") {
    REQUIRE(minWindow("ADOBECODEBANC", "ABC") == "BANC");
    REQUIRE(minWindow("a", "a") == "a");
    REQUIRE(minWindow("a", "aa").empty());
}
