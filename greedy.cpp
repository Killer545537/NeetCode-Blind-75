#include <bits/stdc++.h>
#include <catch2/catch_test_macros.hpp>

int maxSubArray(const std::vector<int>& nums) {
    int maximum_sum = INT_MIN, sum = 0;

    for (const int num : nums) {
        sum += num;
        maximum_sum = std::max(maximum_sum, sum);

        if (sum < 0) sum = 0;
    }

    return maximum_sum;
}

TEST_CASE("Maximum Subarray") {
    REQUIRE(maxSubArray({-2,1,-3,4,-1,2,1,-5,4}) == 6);
    REQUIRE(maxSubArray({1}) == 1);
    REQUIRE(maxSubArray({5,4,-1,7,8}) == 23);
}

bool canJump(const std::vector<int>& jumps) {
    int maximum_index = 0;

    for (size_t i = 0; i < jumps.size(); ++i) {
        if (i > maximum_index) return false;

        const int jump = jumps[i];
        maximum_index = std::max(maximum_index, static_cast<int>(i + jump));

        if (maximum_index >= jumps.size() - 1) return true;
    }

    return true;
}

TEST_CASE("Jump Game") {
    REQUIRE(canJump({2,3,1,1,4}) == true);
    REQUIRE(canJump({3,2,1,0,4}) == false);
}
