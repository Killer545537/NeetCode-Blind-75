#include <bits/stdc++.h>
#include <catch2/catch_test_macros.hpp>

bool isPalindrome(const std::string& s) {
    const auto process = [](const std::string& string) {
        return string | std::views::filter([](const char c) { return std::isalnum(c); })
            | std::views::transform([](const char c) { return static_cast<char>(std::tolower(c)); })
            | std::ranges::to<std::string>();
    };

    const auto processed = process(s);
    for (int i = 0, j = static_cast<int>(processed.length()) - 1; i <= j; i++, j--) {
        if (processed[i] != processed[j]) return false;
    }

    return true;
}

TEST_CASE("Valid Palindrome") {
    REQUIRE(isPalindrome("A man, a plan, a canal: Panama") == true);
    REQUIRE(isPalindrome("race a car") == false);
    REQUIRE(isPalindrome(" ") == true);
}

std::vector<std::vector<int>> threeSum(const std::vector<int>& nums) {
    const auto sorted_nums = [&nums]() {
        auto sorted = nums;
        std::ranges::sort(sorted);

        return sorted;
    }();

    std::vector<std::vector<int>> ans;
    for (int i = 0; i < nums.size(); ++i) {
        if (i > 0 and sorted_nums[i] == sorted_nums[i - 1]) continue;

        int j = i + 1, k = static_cast<int>(nums.size()) - 1;
        while (j < k) {
            if (const int sum = sorted_nums[i] + sorted_nums[j] + sorted_nums[k]; sum > 0) k--;
            else if (sum < 0) j++;
            else {
                ans.push_back({sorted_nums[i], sorted_nums[j], sorted_nums[k]});
                j++;

                while (nums[j] == nums[j - 1] and j < k) j++;
            }
        }
    }

    return ans;
}

TEST_CASE("3Sum") {
    auto sortTriplets = [](std::vector<std::vector<int>> triplets) {
        for (auto& triplet : triplets) std::ranges::sort(triplet);
        std::ranges::sort(triplets);
        return triplets;
    };

    REQUIRE(
        sortTriplets(threeSum({-1,0,1,2,-1,-4})) ==
        sortTriplets({{-1,-1,2}, {-1,0,1}})
    );
    REQUIRE(
        sortTriplets(threeSum({0,1,1})) ==
        sortTriplets({})
    );
    REQUIRE(
        sortTriplets(threeSum({0,0,0})) ==
        sortTriplets({{0,0,0}})
    );
}

int maxArea(const std::vector<int>& heights) {
    size_t left = 0, right = heights.size() - 1;
    int max_area = 0;

    while (left < right) {
        const int length = std::min(heights[left], heights[right]), width = static_cast<int>(right - left);

        max_area = std::max(max_area, length * width);

        if (heights[left] > heights[right]) right--;
        else left++;
    }

    return max_area;
}

TEST_CASE("Container With Most Water") {
    REQUIRE(maxArea({1,8,6,2,5,4,8,3,7}) == 49);
    REQUIRE(maxArea({1,1}) == 1);
}
