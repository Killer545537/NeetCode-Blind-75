#include <bits/stdc++.h>
#include <catch2/catch_template_test_macros.hpp>

class MedianFinder {
    /// This contains the smaller elements
    std::priority_queue<int> smaller; // Max-Heap
    /// This contains the larger elements
    std::priority_queue<int, std::vector<int>, std::greater<>> larger; // Min-Heap
public:
    MedianFinder() = default;

    void addNum(const int num) {
        smaller.push(num);

        // Here, we check if ever num in small is <= every num in larger
        if (!larger.empty() and smaller.top() > larger.top()) {
            larger.push(smaller.top());
            smaller.pop();
        }

        if (smaller.size() > larger.size() + 1) {
            larger.push(smaller.top());
            smaller.pop();
        } else if (larger.size() > smaller.size()) {
            smaller.push(larger.top());
            larger.pop();
        }
    }

    [[nodiscard]] double findMedian() const {
        if (smaller.size() > larger.size()) return smaller.top();
        if (larger.size() > smaller.size()) return larger.top();

        return static_cast<double>(smaller.top() + larger.top()) / 2.0;
    }
};

TEST_CASE("Find Median from Data Stream") {
    MedianFinder median_finder;

    median_finder.addNum(1);
    median_finder.addNum(2);

    REQUIRE(median_finder.findMedian() == 1.5);

    median_finder.addNum(3);

    REQUIRE(median_finder.findMedian() == 2.0);
}
