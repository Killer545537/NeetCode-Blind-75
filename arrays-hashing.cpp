#include <bits/stdc++.h>
#include <catch2/catch_test_macros.hpp>

bool containsDuplicate(const std::vector<int>& nums) {
    std::unordered_set<int> set;

    for (const int num : nums) {
        if (set.contains(num)) return true;
        set.insert(num);
    }

    return false;
}

TEST_CASE("Contains Duplicate") {
    REQUIRE(containsDuplicate({1,2,3,1}) == true);
    REQUIRE(containsDuplicate({1,2,3,4}) == false);
    REQUIRE(containsDuplicate({1,1,1,3,3,4,3,2,4,2}) == true);
}

bool isAnagram(const std::string& s, const std::string& t) {
    if (s.length() != t.length()) return false;

    const auto make_map = [](const std::string& string) {
        std::unordered_map<char, int> map;
        for (const char c : string) map[c]++;

        return map;
    };

    return make_map(s) == make_map(t);
}

TEST_CASE("Valid Anagram") {
    REQUIRE(isAnagram("anagram", "nagaram") == true);
    REQUIRE(isAnagram("rat", "car") == false);
}

std::vector<int> twoSum(const std::vector<int>& nums, const int target) {
    /// nums[i], i
    std::unordered_map<int, size_t> map;

    for (size_t i = 0; i < nums.size(); ++i) {
        if (const int remaining_amount = target - nums[i]; map.contains(remaining_amount))
            return {
                static_cast<int>(map[remaining_amount]), static_cast<int>(i)
            };
        map[nums[i]] = i;
    }

    // One solution exists (problem constraints)
    std::unreachable();
}

TEST_CASE("Two Sum") {
    REQUIRE(twoSum({2,7,11,15}, 9) == std::vector{0, 1});
    REQUIRE(twoSum({3,2,4}, 6) == std::vector{1, 2});
    REQUIRE(twoSum({3,3}, 6) == std::vector{0, 1});
}

std::vector<std::vector<std::string>> groupAnagrams(const std::vector<std::string>& strs) {
    std::unordered_map<std::string, std::vector<std::string>> anagram_group_map;

    for (const std::string& str : strs) {
        std::string key = str;
        std::ranges::sort(key);
        anagram_group_map[key].emplace_back(str);
    }

    std::vector<std::vector<std::string>> anagram_groups;
    anagram_groups.reserve(anagram_group_map.size());
    for (const auto& strings : anagram_group_map | std::views::values) anagram_groups.emplace_back(strings);

    return anagram_groups;
}

TEST_CASE("Group Anagrams") {
    /// Just to remove ordering
    auto sortGroupsForComparison = [](std::vector<std::vector<std::string>> groups) {
        for (auto& group : groups) {
            std::ranges::sort(group);
        }
        std::ranges::sort(groups);
        return groups;
    };

    REQUIRE(sortGroupsForComparison(groupAnagrams({"eat", "tea", "tan", "ate", "nat", "bat"})) ==
        sortGroupsForComparison(std::vector<std::vector<std::string>>{
            {"eat", "tea", "ate"},
            {"tan", "nat"},
            {"bat"}
            }));
    REQUIRE(sortGroupsForComparison(groupAnagrams({""})) ==
        sortGroupsForComparison(std::vector<std::vector<std::string>>{{""}})
    );
    REQUIRE(sortGroupsForComparison(groupAnagrams({"a"})) ==
        sortGroupsForComparison(std::vector<std::vector<std::string>>{{"a"}})
    );
}

std::vector<int> topKFrequent(const std::vector<int>& nums, const int k) {
    // nums[i], freq
    std::unordered_map<int, int> freq_map;
    for (const int num : nums) freq_map[num]++;

    std::vector<std::vector<int>> buckets(nums.size() + 1);
    for (const auto& [num, freq] : freq_map) buckets[freq].push_back(num);

    std::vector<int> result;
    result.reserve(k);

    for (int freq = static_cast<int>(buckets.size()) - 1; freq >= 0 and result.size() < k; --freq) {
        for (int num : buckets[freq]) {
            result.push_back(num);
            if (result.size() == k) break;
        }
    }

    return result;
}

TEST_CASE("Top K Frequent Elements") {
    REQUIRE(topKFrequent({1,1,1,2,2,3}, 2) == std::vector{1, 2});
    REQUIRE(topKFrequent({1}, 1) == std::vector{1});
}

class Encode_Decode {
public:
    static std::string encode(const std::vector<std::string>& strs) {
        std::string encoded{};

        for (const std::string& str : strs)
            encoded += std::format("{}#{}", str.length(), str);

        return encoded;
    }

    static std::vector<std::string> decode(const std::string& s) {
        std::vector<std::string> decoded;
        size_t i = 0;

        while (i < s.length()) {
            size_t j = i;
            while (s[j] != '#') j++;

            const int length = std::stoi(s.substr(i, j - i));
            decoded.emplace_back(s.substr(j + 1, length));

            i = j + 1 + length;
        }

        return decoded;
    }
};

TEST_CASE("Encode Decode Strings") {
    SECTION("Basic functionality") {
        std::vector<std::string> strs = {"Hello", "World"};
        std::string encoded = Encode_Decode::encode(strs);
        REQUIRE(encoded == "5#Hello5#World");
        REQUIRE(Encode_Decode::decode(encoded) == strs);
    }

    SECTION("Empty strings") {
        std::vector<std::string> strs = {""};
        std::string encoded = Encode_Decode::encode(strs);
        REQUIRE(encoded == "0#");
        REQUIRE(Encode_Decode::decode(encoded) == strs);

        strs = {"", "", ""};
        encoded = Encode_Decode::encode(strs);
        REQUIRE(encoded == "0#0#0#");
        REQUIRE(Encode_Decode::decode(encoded) == strs);
    }

    SECTION("Empty vector") {
        std::vector<std::string> strs = {};
        std::string encoded = Encode_Decode::encode(strs);
        REQUIRE(encoded.empty());
        REQUIRE(Encode_Decode::decode(encoded) == strs);
    }

    SECTION("Special characters") {
        std::vector<std::string> strs = {"a#b", "c#d#e"};
        REQUIRE(Encode_Decode::decode(Encode_Decode::encode(strs)) == strs);
    }

    SECTION("Mixed lengths") {
        std::vector<std::string> strs = {"", "a", "ab", "abc", "abcd"};
        REQUIRE(Encode_Decode::decode(Encode_Decode::encode(strs)) == strs);
    }

    SECTION("Long strings") {
        std::vector<std::string> strs = {"This is a very long string that needs to be encoded"};
        REQUIRE(Encode_Decode::decode(Encode_Decode::encode(strs)) == strs);
    }
}

std::vector<int> productExceptSelf(const std::vector<int>& nums) {
    std::vector ans(nums.size(), 1);

    int current = 1;
    for (int i = 0; i < nums.size(); ++i) {
        ans[i] *= current;
        current *= nums[i];
    }

    current = 1;
    for (int i = static_cast<int>(nums.size()) - 1; i >= 0; --i) {
        ans[i] *= current;
        current *= nums[i];
    }

    return ans;
}

TEST_CASE("Product of Array Except Self") {
    REQUIRE(productExceptSelf({1,2,3,4}) == std::vector{24, 12, 8, 6});
    REQUIRE(productExceptSelf({-1,1,0,-3,3}) == std::vector{0, 0, 9, 0, 0});
}

int longestConsecutive(const std::vector<int>& nums) {
    if (nums.empty()) return 0;

    const std::unordered_set set(nums.begin(), nums.end());

    int longest_sequence = 1;
    for (const int num : set) {
        if (!set.contains(num - 1)) {
            int current = num;
            int current_sequence_length = 1;

            while (set.contains(current + 1)) {
                current++;
                current_sequence_length++;
            }

            longest_sequence = std::max(longest_sequence, current_sequence_length);
        }
    }

    return longest_sequence;
}

TEST_CASE("Longest Consecutive Sequence") {
    REQUIRE(longestConsecutive({100,4,200,1,3,2}) == 4);
    REQUIRE(longestConsecutive({0,3,7,2,5,8,4,6,0,1}) == 9);
    REQUIRE(longestConsecutive({1,0,1,2}) == 3);
}
