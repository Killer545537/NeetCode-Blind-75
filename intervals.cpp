#include <bits/stdc++.h>
#include <catch2/catch_test_macros.hpp>

std::vector<std::vector<int>> insert(const std::vector<std::vector<int>>& intervals,
                                     std::vector<int> newInterval) {
    if (intervals.empty()) return {newInterval};

    const size_t LEN = intervals.size();

    // Find position to insert newInterval
    int left = 0, right = static_cast<int>(LEN - 1);
    const int target = newInterval[0];
    while (left <= right) {
        if (const int mid = left + (right - left) / 2; intervals[mid][0] < target) left = mid + 1;
        else right = mid - 1;
    }

    // Merge overlapping intervals
    std::vector<std::vector<int>> res;
    size_t ind = 0;
    while (ind < LEN and intervals[ind][1] < newInterval[0]) {
        res.push_back(intervals[ind]);
        ind++;
    }

    while (ind < LEN and intervals[ind][0] <= newInterval[1]) {
        newInterval[0] = std::min(newInterval[0], intervals[ind][0]);
        newInterval[1] = std::max(newInterval[1], intervals[ind][1]);
        ind++;
    }

    res.push_back(newInterval);

    while (ind < LEN) {
        res.push_back(intervals[ind]);
        ind++;
    }

    return res;
}

TEST_CASE("Insert Interval") {
    REQUIRE(insert({{1,3},{6,9}}, {2,5})
            == std::vector<std::vector<int>>{{1, 5}, {6, 9}});
    REQUIRE(insert({{1,2},{3,5},{6,7},{8,10},{12,16}}, {4,8})
            == std::vector<std::vector<int>>{{1, 2}, {3, 10}, {12, 16}});
}

std::vector<std::vector<int>> merge(std::vector<std::vector<int>> intervals) {
    std::ranges::sort(intervals);
    std::vector<std::vector<int>> merged;
    merged.push_back(intervals[0]);

    for (const auto& interval : intervals) {
        if (const auto start = interval[0], end = interval[1], last_end = merged.back()[1];
            start <= last_end)
            merged.back()[1] = std::max(last_end, end);
        else merged.push_back({start, end});
    }

    return merged;
}

TEST_CASE("Merge Intervals") {
    REQUIRE(merge({{1, 3}, {2, 6}, {8, 10}, {15, 18}})
            == std::vector<std::vector<int>>{{1, 6}, {8, 10}, {15, 18}});
    REQUIRE(merge({{1,4}, {4,5}}) == std::vector<std::vector<int>>{{1, 5}});
}

int eraseOverlapIntervals(std::vector<std::vector<int>> intervals) {
    std::ranges::sort(intervals, [](const std::vector<int>& a, const std::vector<int>& b) {
        return a[1] < b[1];
    });

    int count = 0, free_time = intervals[0][1];
    for (const auto& interval : intervals | std::views::drop(1)) {
        if (interval[0] < free_time) count++;
        else free_time = interval[1];
    }

    return count;
}

TEST_CASE("Non-overlapping Intervals") {
    REQUIRE(eraseOverlapIntervals({{1, 2}, {2, 3}, {3, 4}, {1, 3}}) == 1);
    REQUIRE(eraseOverlapIntervals({{1, 2}, {1, 2}, {1, 2}}) == 2);
    REQUIRE(eraseOverlapIntervals( {{1, 2}, {2, 3}}) == 0);
}

class Interval {
public:
    int start, end;

    Interval(const int start, const int end) {
        this->start = start;
        this->end = end;
    }
};

bool canAttendMeetings(std::vector<Interval> intervals) {
    if (intervals.empty()) return true;

    std::ranges::sort(intervals, {}, &Interval::end);
    int end = intervals[0].end;

    for (const auto& interval : intervals | std::views::drop(1)) {
        if (interval.start < end) return false;
        end = interval.end;
    }

    return true;
}

TEST_CASE("Meeting Rooms") {
    REQUIRE_FALSE(canAttendMeetings({Interval(0, 30), Interval(5, 10), Interval(15, 20)}));
    REQUIRE(canAttendMeetings({Interval(5, 8), Interval(9, 15)}));
}

int minMeetingRooms(const std::vector<Interval>& intervals) {
    std::vector<int> start = intervals | std::views::transform([](const auto interval) { return interval.start; }) |
        std::ranges::to<std::vector>();
    std::ranges::sort(start);
    std::vector<int> end = intervals | std::views::transform([](const auto interval) { return interval.end; }) |
        std::ranges::to<std::vector>();
    std::ranges::sort(end);

    int rooms = 0, current_meetings = 0;
    int s = 0, e = 0;

    while (s < intervals.size()) {
        if (start[s] < end[e]) {
            s++;
            current_meetings++;
        }
        else {
            e++;
            current_meetings--;
        }

        rooms = std::max(rooms, current_meetings);
    }

    return rooms;
}

TEST_CASE("Meeting Rooms II") {
    REQUIRE(minMeetingRooms({Interval(0, 40), Interval(5, 10), Interval(15, 20)}) == 2);
    REQUIRE(minMeetingRooms({Interval(4, 9)}) == 1);
}
