#include <bits/stdc++.h>
#include <catch2/catch_test_macros.hpp>

int findMin(const std::vector<int>& nums) {
    int left = 0, right = static_cast<int>(nums.size() - 1), min_element = INT_MAX;

    while (left <= right) {
        if (const int mid = (right + left) / 2; nums[left] <= nums[mid]) {
            // If the left portion is sorted
            min_element = std::min(min_element, nums[left]);
            left = mid + 1;
        }
        else {
            // If the right portion is sorted
            min_element = std::min(min_element, nums[mid]);
            right = mid - 1;
        }
    }

    return min_element;
}

TEST_CASE("Find Minimum in Rotated Sorted Array") {
    REQUIRE(findMin({3,4,5,1,2}) == 1);
    REQUIRE(findMin({4,5,6,7,0,1,2}) == 0);
    REQUIRE(findMin({11,13,15,17}) == 11);
}

int search(const std::vector<int>& nums, const int target) {
    int low = 0, high = static_cast<int>(nums.size()) - 1;

    while (low <= high) {
        const int mid = (low + high) / 2;
        if (nums[mid] == target) return mid;

        // If the left portion is sorted
        if (nums[low] <= nums[mid]) {
            if (nums[low] <= target and target <= nums[mid]) high = mid - 1;
            else low = mid + 1;
        }
        // If the right portion is sorted
        else {
            if (nums[mid] <= target and target <= nums[high]) low = mid + 1;
            high = mid - 1;
        }
    }

    return -1;
}

TEST_CASE("Search in Rotated Sorted Array") {
    REQUIRE(search({4,5,6,7,0,1,2}, 0) == 4);
    REQUIRE(search({4,5,6,7,0,1,2}, 3) == -1);
    REQUIRE(search({1}, 0) == -1);
}