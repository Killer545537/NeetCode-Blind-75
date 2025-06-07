#include <bits/stdc++.h>
#include <catch2/catch_test_macros.hpp>

int climbStairs(const int n) {
    if (n == 1) return 1;
    if (n == 2) return 2;

    int one_step = 1, two_step = 2, total_steps = 0;
    for (int i = 3; i <= n; ++i) {
        total_steps = one_step + two_step;
        one_step = two_step;
        two_step = total_steps;
    }

    return two_step;
}

TEST_CASE("Climbing Stairs") {
    REQUIRE(climbStairs(2) == 2);
    REQUIRE(climbStairs(3) == 3);
}

int rob(const std::vector<int>& houses) {
    int prev1 = 0, prev2 = 0;

    for (const int house : houses) {
        int take = house + prev2, not_take = prev1, current = std::max(take, not_take);

        prev2 = prev1;
        prev1 = current;
    }

    return prev1;
}

TEST_CASE("House Robber") {
    REQUIRE(rob({1,2,3,1}) == 4);
    REQUIRE(rob({2,7,9,3,1}) == 12);
}

int rob2(const std::vector<int>& houses) {
    if (houses.size() == 1) return houses[0];

    return std::max(rob(std::vector(houses.begin() + 1, houses.end())),
                    rob(std::vector(houses.begin(), houses.end() - 1)));
}

TEST_CASE("House Robber II") {
    REQUIRE(rob2({2,3,2}) == 3);
    REQUIRE(rob2({1,2,3,1}) == 4);
    REQUIRE(rob2({1,2,3}) == 3);
}

std::string longestPalindrome(const std::string& s) {
    const size_t LEN = s.length();
    std::vector dp(LEN, std::vector(LEN, false));
    std::pair end_points{0, 0};

    for (size_t i = 0; i < LEN; ++i) dp[i][i] = true;

    for (size_t i = 0; i < LEN - 1; ++i)
        // Every consecutive character is a palindrome
        if (s[i] == s[i + 1]) {
            dp[i][i + 1] = true;
            end_points = {i, i + 1};
        }

    for (int difference = 2; difference < LEN; ++difference) {
        for (size_t i = 0; i < LEN - difference; ++i) {
            if (const size_t j = i + difference; s[i] == s[j] and dp[i + 1][j - 1]) {
                dp[i][j] = true;
                end_points = {i, j};
            }
        }
    }

    const auto [start, end] = end_points;
    return s.substr(start, end - start + 1);
}

TEST_CASE("Longest Palindromic Substring") {
    REQUIRE(((longestPalindrome("babad") == "bab") || (longestPalindrome("babad") == "aba")));
    REQUIRE(longestPalindrome("cbbd") == "bb");
}

int countSubstrings(const std::string& s) {
    int count = 0;

    for (int i = 0; i < s.length(); ++i) {
        int left = i, right = i;
        while (left >= 0 and right < s.length() and s[left] == s[right]) {
            count++;
            left--;
            right++;
        }
        left = i, right = i + 1;
        while (left >= 0 and right < s.length() and s[left] == s[right]) {
            count++;
            left--;
            right++;
        }
    }

    return count;
}

TEST_CASE("Palindromic Substrings") {
    REQUIRE(countSubstrings("abc") == 3);
    REQUIRE(countSubstrings("aaa") == 6);
}

int numDecodings(const std::string& s) {
    std::vector dp(s.length() + 1, 0);
    dp[s.length()] = 1;

    for (int i = s.length() - 1; i >= 0; --i) {
        if (s[i] == '0') dp[i] = 0;
        else {
            int res = dp[i + 1];

            if (i + 1 < s.length() and
                (s[i] == '1' or (
                    s[i] == '2' and std::set{'0', '1', '2', '3', '4', '5', '6'}.contains(s[i + 1])
                ))
            )
                res += dp[i + 2];

            dp[i] = res;
        }
    }

    return dp[0];
}

TEST_CASE("Decode Ways") {
    REQUIRE(numDecodings("12") == 2);
    REQUIRE(numDecodings("226") == 3);
    REQUIRE(numDecodings("06") == 0);
}

int coinChange(const std::vector<int>& coins, const int amount) {
    std::vector dp(amount + 1, INT_MAX);
    dp[0] = 0;

    for (const int coin : coins)
        for (int i = coin; i <= amount; ++i)
            if (dp[i - coin] != INT_MAX) dp[i] = std::min(dp[i], dp[i - coin] + 1);

    return dp[amount] == INT_MAX ? -1 : dp[amount];
}

TEST_CASE("Coin Change") {
    REQUIRE(coinChange({1,2,5}, 11) == 3);
    REQUIRE(coinChange({2}, 3) == -1);
    REQUIRE(coinChange({1}, 0) == 0);
}

int maxProduct(const std::vector<int>& nums) {
    const int LEN = static_cast<int>(nums.size());
    int prefix = 1, suffix = 1, maximum_product = INT_MIN;

    for (int i = 0; i < LEN; ++i) {
        if (prefix == 0) prefix = 1;
        if (suffix == 0) suffix = 1;

        prefix *= nums[i];
        suffix *= nums[LEN - i - 1];

        maximum_product = std::ranges::max({
            maximum_product, prefix, suffix
        });
    }

    return maximum_product;
}

TEST_CASE("Maximum Product Subarray") {
    REQUIRE(maxProduct({2,3,-2,4}) == 6);
    REQUIRE(maxProduct({-2,0,-1}) == 0);
}

bool wordBreak(const std::string& word, const std::vector<std::string>& wordDict) {
    std::vector dp(word.length() + 1, false);
    dp[word.length()] = true;

    for (int i = word.length() - 1; i >= 0; --i) {
        for (const std::string& key : wordDict) {
            if (i + key.length() <= word.length() and word.substr(i, key.length()) == key)
                dp[i] = dp[i + key.length()];
            if (dp[i]) break;
        }
    }

    return dp[0];
}

TEST_CASE("Word Break") {
    REQUIRE(wordBreak("leetcode", {"leet","code"}) == true);
    REQUIRE(wordBreak("applepenapple", {"apple","pen"}) == true);
    REQUIRE(wordBreak("catsandog", {"cats","dog","sand","and","cat"}) == false);
}
