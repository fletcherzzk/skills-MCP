# Functional review: scores.cpp

## 1. Summary

- Errors: 5
- Warnings: 3
- Suggestions: 1
- Unverified findings: 0

Scope: static review of `scores.cpp`, including types, contracts, arithmetic, mutation, side effects, and iteration. No source or existing project files were modified. No C++ compiler was available on PATH; examples below are derived from the code, not executable test results.

## 2. Findings

### [ERROR] Ranking changes the caller's input

- Location: `scores.cpp:27-32` (`build_ranking`)
- Issue: The mutable reference and in-place sort reorder `students`, violating the explicit requirement at line 25. The sample moves Cara ahead of Alice and moves Bob toward the end.
- Recommendation: Accept `const std::vector<Student>&`, construct the output entries in input order, then sort the output. This also makes mutation of the caller's vector impossible through this parameter.

### [ERROR] Equal-score order is not preserved by contract

- Location: `scores.cpp:29` (`build_ranking`)
- Issue: `std::sort` does not guarantee retention of the original order for equivalent elements. This fails to implement the explicit tie-order requirement at line 24. A particular run preserving Alice and Dan's order does not establish correctness.
- Recommendation: Use `std::stable_sort` on the output entries with a descending score comparator.

### [ERROR] Individual scores are truncated before averaging

- Location: `scores.cpp:49-52` (`average_score`)
- Issue: Casting each score to `int` loses its fractional component. For example, homework 81 and exam 80 produce a score of 80.4, but contribute only 80 to the total.
- Recommendation: Accumulate the original scores into a `double`, initialized with `0.0`. If using `std::accumulate`, its initial value must also be `0.0` so the accumulator is floating point.

### [ERROR] Average uses integer division

- Location: `scores.cpp:55` (`average_score`)
- Issue: Both operands are integral, so division discards the fractional part before conversion to the function's `double` return type. The sample scores total 343, making the expected average 85.75; this expression produces 85.
- Recommendation: Divide a floating-point total by the input size after checking for empty input. Changing the return type alone does not change how the expression is evaluated.

### [ERROR] Empty input divides by zero

- Location: `scores.cpp:55` (`average_score`)
- Issue: An empty vector makes the divisor zero. Integral division by zero is undefined behavior and violates the documented requirement to return 0.0.
- Recommendation: Return `0.0` immediately when `students.empty()`.

### [WARNING] Integer accumulation can overflow

- Location: `scores.cpp:49-52` (`average_score`)
- Issue: The signed `int` total can overflow for sufficiently many students even with scores restricted to 0 through 100. Signed overflow is undefined behavior; on a 32-bit int platform, 21,474,837 scores of 100 exceed the limit.
- Recommendation: Use the floating-point accumulator recommended above. Document any additional score-domain assumptions required by callers.

### [WARNING] Signed and unsigned division mishandles negative totals

- Location: `scores.cpp:55` (`average_score`)
- Issue: The total is `int`, while `students.size()` is an unsigned size type. On typical platforms, the usual arithmetic conversions turn a negative total into a large unsigned value before division. The `Student` API accepts negative integers and declares no score-range restriction.
- Recommendation: Use floating-point accumulation and division. If negative grades are invalid for the application, define and enforce that contract at the input boundary.

### [WARNING] Passing count can exceed its return type

- Location: `scores.cpp:59-67` (`count_passing`)
- Issue: The vector can contain more than `INT_MAX` passing students, but both the accumulator and return value are `int`. Incrementing past that limit causes signed overflow. This is a capacity-dependent risk, not a problem demonstrated by the sample.
- Recommendation: Return and accumulate a suitable size type such as `std::vector<Student>::size_type`, or explicitly check and enforce an `int` count limit. Replacing the loop with `std::count_if` still requires checking the result type and any narrowing conversion.

### [SUGGESTION] Compute ranking scores once and construct entries directly

- Location: `scores.cpp:29-40` (`build_ranking`)
- Issue: The comparator repeatedly computes scores during sorting, and each output entry is default-constructed, assigned field by field, then copied into the vector. These are optional efficiency and clarity improvements; local mutation itself is not a correctness problem.
- Recommendation: Reserve `students.size()` output capacity, append aggregate entries such as `{student.name, final_score(student)}` in a range-based loop or `std::transform`, then stable-sort the entries by their cached score. This also implements the fixes for input preservation and stable ties without an extra student-vector copy.

## 3. Other checks

- `final_score` is pure and uses floating-point multiplication before addition, so it does not first perform an overflowing integer weighted sum. Its usual floating-point rounding is not itself a defect under the stated contract.
- The comparator's descending relation is appropriate for these finite scores. The defect is the missing stability guarantee.
- The loops use correct bounds and terminate for fixed input. There is no recursion requiring stack analysis or a CPS transformation.
- `count_passing` correctly includes equality at the threshold for finite ordinary inputs, subject to the count capacity warning above.
- Console output is confined to `main`, where it is intentional. Higher-order algorithms are optional; replacing these short loops is not required for functional safety.
