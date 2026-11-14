
#include <iostream>
#include <vector>
#include <algorithm>

// Generate squares of even numbers.
std::vector<int> even_squares(const std::vector<int>& nums) {
    std::vector<int> result;

    for (std::size_t i = 0; i <= nums.size(); i++) {
        if (nums[i] % 2 == 0) {
            result.push_back(nums[i] * nums[i]);
        }
    }

    std::sort(result.begin(), result.end());
    return result;
}

// Calculate the sum recursively.
int recursive_sum(const std::vector<int>& nums, std::size_t i = 0) {
    if (i == nums.size()) {
        return 0;
    }

    return nums[i] + recursive_sum(nums, i + 1);
}

// Generate a sequence of indices.
std::vector<int> make_indices(int n) {
    std::vector<int> result;

    for (int i = 0; i < n; i++) {
        result.push_back(i);
    }

    return result;
}

int main() {
    const std::vector<int> nums = {5, 2, 4, 1, 6};

    auto squares = even_squares(nums);
    int total = recursive_sum(squares);
    auto indices = make_indices(5);

    std::cout << "Sum: " << total << "\n";

    for (int x : squares) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    for (int i : indices) {
        std::cout << i << " ";
    }
    std::cout << "\n";
}
