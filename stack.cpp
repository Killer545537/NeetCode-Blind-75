#include <bits/stdc++.h>
#include <catch2/catch_test_macros.hpp>

bool isValid(const std::string& s) {
    const std::unordered_map<char, char> matching_parenthesis{
        {')', '('}, {']', '['}, {'}', '{'}
    };

    std::stack<char> stack;
    for (const char parenthesis : s) {
        if (!matching_parenthesis.contains(parenthesis)) stack.push(parenthesis);
        else if (stack.empty() or stack.top() != matching_parenthesis.at(parenthesis)) return false;
        else stack.pop();
    }

    return true;
}

TEST_CASE("Valid Parentheses") {
    REQUIRE(isValid("()") == true);
    REQUIRE(isValid("()[]{}") == true);
    REQUIRE(isValid("(]") == false);
    REQUIRE(isValid("([])") == true);
}

int carFleet(const int target, const std::vector<int>& position, const std::vector<int>& speed) {
    const auto make_cars = [target](const std::vector<int>& pos, const std::vector<int>& s) {
        /// Position, time to get to the end
        std::vector<std::pair<int, float>> cars;
        cars.reserve(pos.size());

        for (size_t i = 0; i < pos.size(); ++i)
            cars.emplace_back(
                pos[i], static_cast<float>(target - pos[i]) / static_cast<float>(s[i]));

        std::ranges::sort(cars);
        return cars;
    };

    const auto cars = make_cars(position, speed);

    std::stack<float> fleets;
    for (const auto& time : cars | std::views::values) {
        const float current_fleet = time;

        while (!fleets.empty() and current_fleet >= fleets.top()) fleets.pop();

        fleets.push(current_fleet);
    }

    return static_cast<int>(fleets.size());
}
