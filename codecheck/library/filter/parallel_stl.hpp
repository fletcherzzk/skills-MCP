#pragma once

#include <algorithm>
#include <execution>
#include <vector>

namespace functional_cpp {

// Stable filtering: preserves the relative order of elements.
// O(n) predicate applications; no mutation of xs.
// pred must be pure and safe to invoke concurrently.
// Limitation: T must be default-constructible, copyable and assignable.
template <class T, class Pred>
std::vector<T> parallel_filter(const std::vector<T>& xs, Pred pred) {
    std::vector<T> result(xs.size());
    const auto new_end = std::copy_if(std::execution::par, xs.begin(),
                                      xs.end(), result.begin(), pred);
    result.erase(new_end, result.end());
    return result;
}

} // namespace functional_cpp
