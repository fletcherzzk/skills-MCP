#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>

// Arithmetic contract: results and every intermediate sum must fit in int.
// Overflow throws std::overflow_error before arithmetic is performed.
// No assumption is made about the bit width of int.
namespace {
int checked_square(int value) {
    const int maximum = std::numeric_limits<int>::max();
    // Division avoids overflowing multiplication or negating INT_MIN.
    if ((value > 0 && value > maximum / value) ||
        (value < 0 && value < maximum / value)) {
        throw std::overflow_error("square exceeds int range");
    }
    return value * value;
}

int checked_add(int left, int right) {
    if ((right > 0 && left > std::numeric_limits<int>::max() - right) ||
        (right < 0 && left < std::numeric_limits<int>::min() - right)) {
        throw std::overflow_error("sum exceeds int range");
    }
    return left + right;
}
} // namespace

// Accepts any int input; odd values are ignored. Each selected square must
// fit in int (for 32-bit int, selected even values must be in [-46340, 46340]).
// Fused sequential filter/map avoids a materialized filtered vector.
std::vector<int> even_squares(const std::vector<int>& nums) {
    std::vector<int> result;
    for (const int value : nums) {
        if (value % 2 == 0) {
            result.push_back(checked_square(value));
        }
    }
    std::sort(result.begin(), result.end());
    return result;
}

// Retains the original API name but uses constant stack space.
// i == size returns zero; i > size throws std::out_of_range.
// Fold from the right to preserve the recursive addition grouping, including
// which mixed-sign inputs overflow. Every suffix sum must fit in int.
int recursive_sum(const std::vector<int>& nums, std::size_t i = 0) {
    if (i > nums.size()) {
        throw std::out_of_range("sum starting index exceeds vector size");
    }
    using Difference = std::vector<int>::difference_type;
    const auto suffix_begin = nums.begin() + static_cast<Difference>(i);
    return std::accumulate(nums.rbegin(),
                           std::make_reverse_iterator(suffix_begin),
                           0, checked_add);
}

// Preserve the empty result for n <= 0. Positive n produces [0, n).
// n is int, so iota's final increment to n is also representable.
// Allocation failures propagate as standard library exceptions.
std::vector<int> make_indices(int n) {
    if (n <= 0) {
        return {};
    }
    std::vector<int> result(static_cast<std::size_t>(n));
    std::iota(result.begin(), result.end(), 0);
    return result;
}

int main() {
    try {
        const std::vector<int> nums = {5, 2, 4, 1, 6};
        const auto squares = even_squares(nums);
        const int total = recursive_sum(squares);
        const auto indices = make_indices(5);

        std::cout << "Sum: " << total << "\n";
        for (const int x : squares) {
            std::cout << x << " ";
        }
        std::cout << "\n";
        for (const int i : indices) {
            std::cout << i << " ";
        }
        std::cout << "\n";
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << "\n";
        return 1;
    }
    return 0;
}
