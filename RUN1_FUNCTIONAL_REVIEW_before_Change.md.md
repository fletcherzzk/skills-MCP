# Functional review: test.cpp

## 1. Summary

- Errors: **1**
- Warnings: **4**
- Suggestions: **2**
- Unverified findings: **0**

The immediate defect is an out-of-bounds read in `even_squares`, including for the input in `main`. Other findings concern unrestricted inputs, arithmetic overflow, recursion depth, and optional clarity improvements. Warning triggers are confirmed by inspection; they are not all exercised by the sample input.

Validation: static inspection only. No C++ compiler (`cl`, `clang++`, `g++`, or `c++`) was available on PATH, so compilation and runtime checks were not performed. No source files were modified. Zero unverified findings means no speculative findings are listed; it does not imply runtime validation.

## 2. Findings

### [ERROR] E1: Loop reads past the vector's end

- Location: `test.cpp:10` (`even_squares`), with the invalid access at line 11.
- Issue: `i <= nums.size()` permits `i == nums.size()`, where `nums[i]` is outside the valid range. This is undefined behavior for the sample input and for an empty vector. Output cannot be relied upon.
- Recommendation: Use `i < nums.size()` or iterate over values with a range-based loop. Check empty and nonempty inputs with a checked standard-library build or AddressSanitizer.

### [WARNING] W1: Squaring can overflow signed int

- Location: `test.cpp:12` (`even_squares`).
- Issue: Both operands and the destination element type are `int`. An even input whose square exceeds `INT_MAX` causes signed overflow and undefined behavior. With a 32-bit `int`, `{46342}` is one triggering input. There is no documented or enforced magnitude bound.
- Recommendation: Define the supported input range and check multiplication before performing it, or widen the operands before multiplying and return an appropriately wider element type. Widening only after multiplication does not prevent overflow. If using a fixed-width wider type, establish that it can represent every supported square.

### [WARNING] W2: Recursive addition can overflow signed int

- Location: `test.cpp:26` (`recursive_sum`).
- Issue: The returned sum and each intermediate addition have type `int`. On a 32-bit implementation, `{INT_MAX, 1}` triggers undefined behavior. Even individually representable squares can have a sum exceeding the range; two squares of 40000 are an example.
- Recommendation: Choose and document the result range, and use checked addition or an accumulator and return type wide enough for supported inputs. Update `main`'s `total` declaration at line 44 to avoid narrowing a widened return value. A wider fixed-width type still requires bounds for arbitrarily long vectors.

### [WARNING] W3: Linear recursion risks stack exhaustion

- Location: `test.cpp:26` (`recursive_sum`).
- Issue: The function creates a recursive call for every remaining element. Addition occurs after the recursive call returns, so this is not tail recursion. Large inputs can exhaust the stack; compiler optimization cannot be assumed to eliminate the calls.
- Recommendation: Prefer a sequential fold such as `std::accumulate`, or an iterative accumulator, with the overflow policy from W2. An accumulator-style tail-recursive rewrite or CPS rewrite alone does not guarantee bounded stack use in C++. For general signed inputs, account for the change in addition order when defining overflow behavior.

### [WARNING] W4: Public starting index lacks an enforced bound

- Location: `test.cpp:22` (`recursive_sum`), with the invalid access at line 26.
- Issue: The base case handles only `i == nums.size()`. A caller can provide `i > nums.size()` and reach an out-of-bounds access. For example, `recursive_sum({}, 1)` is invalid. The default invocation in `main` does not trigger this defect.
- Recommendation: Define the parameter contract. Reject `i > nums.size()`, or use `i >= nums.size()` if an exhausted suffix should return zero. Alternatively, remove the public index parameter and sum through iterators.

### [SUGGESTION] S1: Express index generation directly

- Location: `test.cpp:30` (`make_indices`), particularly lines 33-34.
- Issue: Repeated `push_back` correctly generates the sequence but requires capacity growth and expresses a standard sequence operation manually.
- Recommendation: For positive `n`, create a vector with `n` elements and fill it with `std::iota` from `<numeric>`. Preserve the current empty result for `n <= 0` before converting to an unsigned size. This avoids repeated capacity growth without introducing parallel execution or an extra index vector.

### [SUGGESTION] S2: Mark read-only local results const

- Location: `test.cpp:43` (`main`), also lines 44-45.
- Issue: `squares`, `total`, and `indices` are never modified after initialization.
- Recommendation: Declare these locals `const` to express their usage and prevent accidental mutation. This is an optional clarity improvement, not a correctness fix.

## Additional review notes

- Type compatibility: index and vector size types in `even_squares` agree. Arithmetic and storage remain `int`, producing the range concerns above. No other unresolved type constraints were identified by inspection.
- Purity and mutation: helper functions read const input references and build local results. Local vector mutation and sorting do not introduce shared mutable state. Console output in `main` is an intentional side effect.
- Ordering: retain the sort at line 16 unless the intended contract changes. Filtering and mapping alone would return results in input order, which can differ from the current sorted order.
- Higher-order alternatives: consulted the connected Functional C++ library's `filter`, `map`, `reduce`, and `tabulate` references. Its implementations require C++17 and request parallel execution. A filter/map pipeline is possible after defining safe arithmetic, but separate materialized stages add allocation and traversal costs compared with a fused loop. Parallel reduction also requires a valid associative operation and identity; overflowing signed addition does not satisfy those requirements. The tabulate reference uses additional index storage, so `std::iota` is simpler for this identity sequence. No unconditional parallel rewrite is recommended for this small program.
- After correcting E1, the supplied values are within the arithmetic range and should produce `Sum: 56`, sorted squares `4 16 36`, and indices `0 1 2 3 4`. This is an expected result from inspection, not observed program output.
