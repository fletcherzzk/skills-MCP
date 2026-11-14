#pragma once

#include <execution>
#include <numeric>
#include <utility>
#include <vector>

namespace functional_cpp {

// O(n) work. Identity and op must form an associative, commutative reduction
// for a deterministic result regardless of grouping/processing order.
// For floating-point arithmetic, expect small rounding differences.
template <class T, class Op>
T parallel_reduce(const std::vector<T>& xs, T identity, Op op) {
    return std::reduce(std::execution::par, xs.begin(), xs.end(),
                       std::move(identity), op);
}

} // namespace functional_cpp
