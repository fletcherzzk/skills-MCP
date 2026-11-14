#pragma once

#include <algorithm>
#include <execution>
#include <functional>
#include <vector>

namespace functional_cpp {

// Returns a sorted copy, leaving the input untouched.
// O(n log n) comparison complexity; NOT a stable sort.
// comp must be a pure, thread-safe strict weak ordering.
template <class T, class Compare = std::less<T>>
std::vector<T> parallel_sort(const std::vector<T>& xs, Compare comp = Compare{}) {
    std::vector<T> result(xs);
    std::sort(std::execution::par, result.begin(), result.end(), comp);
    return result;
}

} // namespace functional_cpp
