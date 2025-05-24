#include <bits/stdc++.h>
#include <tl/enumerate.hpp>

std::vector<int> findWordsContaining(const std::vector<std::string>& words, const char ch) {
    return words | tl::views::enumerate | std::views::filter([&](const auto& pair) {
            return pair.second.contains(ch);
        })
        | std::views::transform([](const auto& pair) {
            return static_cast<int>(pair.first);
        })
        | std::ranges::to<std::vector<int>>();
}

int main() {
    const std::vector<std::string> words = {"leet", "code"};
    constexpr char ch = 'e';

    std::println("{}", findWordsContaining(words, ch));
}
