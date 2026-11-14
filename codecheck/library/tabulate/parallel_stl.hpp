#pragma once

#include <algorithm>
#include <cstddef>
#include <execution>
#include <functional>
#include <numeric>
#include <type_traits>
#include <utility>
#include <vector>

namespace functional_cpp {

// Constructs [f(0), f(1), ..., f(n-1)]; O(n) work and O(n) extra index space.
// f must be pure and safe to invoke concurrently.
// Limitation: result values must be default-constructible and assignable.
template <class F>
auto parallel_tabulate(std::size_t n, F f)
    -> std::vector<std::decay_t<std::invoke_result_t<F&, std::size_t>>> {
    using R = std::decay_t<std::invoke_result_t<F&, std::size_t>>;
    std::vector<std::size_t> indices(n);
    std::iota(indices.begin(), indices.end(), std::size_t{0});
    std::vector<R> result(n);
    std::transform(std::execution::par, indices.begin(), indices.end(),
                   result.begin(), f);
    return result;
}

} // namespace functional_cpp
