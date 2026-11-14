#pragma once

#include <algorithm>
#include <execution>
#include <functional>
#include <type_traits>
#include <utility>
#include <vector>

namespace functional_cpp {

// O(n) work; requests parallel execution through the C++17 execution policy.
// Does not modify xs. f must be pure and safe to invoke concurrently.
// Limitation: result values must be default-constructible and assignable.
template <class T, class F>
auto parallel_map(const std::vector<T>& xs, F f)
    -> std::vector<std::decay_t<std::invoke_result_t<F&, const T&>>> {
    using R = std::decay_t<std::invoke_result_t<F&, const T&>>;
    std::vector<R> result(xs.size());
    std::transform(std::execution::par, xs.begin(), xs.end(), result.begin(), f);
    return result;
}

} // namespace functional_cpp
