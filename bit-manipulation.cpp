#include <bits/stdc++.h>
#include <catch2/catch_test_macros.hpp>

int hammingWeight(int n) {
    // return static_cast<int>(std::bitset<32>(n).count()); This is the best looking solution
    int count = 0;
    while (n != 0) {
        if (n & 1) count++;
        n >>= 1; // Right Shift `n` by 1
    }

    return count;
}

TEST_CASE("Number of 1 Bits") {
    REQUIRE(hammingWeight(11) == 3);
    REQUIRE(hammingWeight(128) == 1);
    REQUIRE(hammingWeight(2147483645) == 30);
}

std::vector<int> countBits(const int n) {
    std::vector<int> bits(n + 1);
    bits[0] = 0;
    for (int i = 1; i <= n; ++i) bits[i] = bits[i / 2] + i % 2;

    return bits;
}

TEST_CASE("Counting Bits") {
    REQUIRE(countBits(2) == std::vector<int>{0, 1, 1});
    REQUIRE(countBits(5) == std::vector<int>{0, 1, 1, 2, 1, 2});
}

uint32_t reverseBits(const uint32_t n) {
    uint32_t reverse = 0;

    for (int i = 0; i < 32; ++i) {
        const uint32_t bit = n >> i & 1;
        reverse |= bit << (31 - i);
    }

    return reverse;
}

TEST_CASE("Reverse Bits") {
    REQUIRE(reverseBits(0b00000010100101000001111010011100) == 964176192);
    REQUIRE(reverseBits(0b11111111111111111111111111111101) == 3221225471);
}

int missingNumber(const std::vector<int>& nums) {
    int XOR = 0, i = 0;
    for (; i < nums.size(); ++i) XOR ^= i ^ nums[i];

    return XOR ^ i;
}

TEST_CASE("Missing Number") {
    REQUIRE(missingNumber({3,0,1}) == 2);
    REQUIRE(missingNumber({0,1}) == 2);
    REQUIRE(missingNumber({9,6,4,2,3,5,7,0,1}) == 8);
}

int getSum(int a, int b) {
    while (b != 0) {
        const int carry = (a & b) << 1;
        a ^= b;
        b = carry;
    }

    return a;
}

TEST_CASE("Sum of Two Integers") {
    REQUIRE(getSum(1, 2) == 3);
    REQUIRE(getSum(2, 3) == 5);
}