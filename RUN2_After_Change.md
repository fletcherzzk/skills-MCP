# Functional Review: score_changed.cpp

## 1. Summary

- Errors: 1
- Warnings: 0
- Suggestions: 0
- Unverified findings: 0
- Scope: `score_changed.cpp`; source files were not modified.

## 2. Findings

### [ERROR] [P2] Round the weighted score once to preserve ties and threshold boundaries

- Location: `score_changed.cpp:19` (`final_score`); consumers at `score_changed.cpp:37` (`build_ranking`) and `score_changed.cpp:66` (`count_passing`).
- Issue: Multiplying the two components separately by inexact binary floating-point constants can produce different results for mathematically equal grades. With ordinary IEEE-754 double arithmetic, homework/exam pairs `(0, 2)` and `(3, 0)` yield `1.2` and `1.2000000000000002`. If supplied in that order, the ranking reverses them despite the documented tie-order guarantee. The same calculation can incorrectly fail a student at an inclusive threshold: `(0, 3)` produces `1.7999999999999998`, which is less than a threshold of `1.8`, although the mathematical grade equals it. These examples use nonnegative integer grades within 0–100.
- Recommendation: Compute the exact integer numerator `2 * homework + 3 * exam` in a type wide enough for the supported input range, then divide once by `5.0`. For the usual 32-bit `int` domain, promote operands to `std::int64_t` before multiplication. This gives equal weighted totals identical floating-point representations and makes these boundary examples compare correctly. Do not introduce an epsilon-based sort comparator, which can violate strict weak ordering.

## 3. Verification and remaining checks

- Reproduced both numerical examples using PowerShell/.NET double arithmetic. No C++ compiler was available on PATH, so the C++ executable was not compiled or run; floating-point contraction and evaluation settings can affect the exact rounding on other builds.
- Static review found compatible types, bounded iteration, preserved input vectors, and appropriate local mutation. The passing counter uses the vector's size type. Multiplication in the current score formula occurs in floating point, avoiding integer multiplication overflow. Empty input is explicitly handled by `average_score`.
- No recursion or avoidable shared mutable state was identified. Standard output is confined to `main`.
- Consulted the functional C++ library's `reduce` reference. Its parallel reduction requires associative and commutative operations for deterministic grouping; floating-point addition does not satisfy that contract exactly. No parallel reduction refactor is recommended.
