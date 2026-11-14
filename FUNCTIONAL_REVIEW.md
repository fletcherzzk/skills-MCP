# Functional Review

## 1. Summary

Reviewed the user-written code in `test.cpp` using `my-skill/skill.md`.

| Classification | Count |
| --- | ---: |
| Errors | 0 |
| Warnings | 2 |
| Suggestions | 1 |
| Unverified findings | 3 |

No confirmed correctness errors were identified by static inspection. Compilation and execution were not performed: `parallel.h` is absent from this repository, and no C++ compiler was found on PATH. Library-dependent conclusions below remain unverified. Source files were not modified.

## 2. Findings

### [WARNING] Missing direct include for the maximum algorithm

- Location: `test.cpp:48` (`combine`); include block at `test.cpp:3`.
- Issue: `std::ranges::max` is declared by `<algorithm>`, which this file does not include directly. Compilation currently depends on a transitive include from another header. This is a portability risk, not a confirmed compilation failure without the missing header.
- Recommendation: Include `<algorithm>` explicitly and build in C++20 or later mode, as also required by the constrained placeholder syntax.

### [WARNING] Skyline preconditions are neither documented nor enforced

- Location: `test.cpp:20` (`singleton`), `test.cpp:25` (`combine`), and `test.cpp:29` (`combine`).
- Issue: The algorithm assumes valid building intervals, nonnegative heights, and coordinate sequences ordered by increasing x. The sentinel `-1` represents an absent height, so accepting it as an actual height loses information. For example, `singleton({4, 3, 1})` produces descending coordinates, violating the merge precondition. No supplied caller or contract establishes that these inputs are excluded.
- Recommendation: Document requirements such as `l < r`, `h >= 0`, sorted input coordinates, and a canonical representation for coincident x coordinates. Validate at the appropriate input boundary if callers cannot guarantee them. These checks need not be repeated inside every parallel operation.

### [SUGGESTION] Make lambda dependencies explicit

- Location: `test.cpp:26` (`combine`), with similar capture defaults at lines 27–30 and 36–40.
- Issue: The mapping, comparison, and scan lambdas use `[&]` even though their bodies need no enclosing variables. Their current bodies have no avoidable side effects, but the capture default permits future implicit dependencies.
- Recommendation: Use `[]` for these lambdas. For the lambdas at lines 47 and 53, explicitly capture only the sequences they use, subject to the library's execution and lifetime guarantees.

### [UNVERIFIED] Library concepts and operation signatures

- Location: `test.cpp:6` (dependency), `test.cpp:20` (`singleton`), and `test.cpp:25` (`combine`).
- Issue: The unavailable `parallel.h` prevents checking that `par::sequence` supports the constrained placeholder forms, that `array_sequence<coord>` satisfies the return constraint, and that the merge, scan, tabulation, and indexed-filter signatures match these calls. In particular, the argument order and identity semantics of `scan_inclusive` need confirmation.
- Recommendation: Compile against the intended header and language version. Verify the inferred pipeline types: coordinates for mapped/merged sequences, integers for height/scanned sequences, and coordinates for the returned filtered sequence.

### [UNVERIFIED] Escaping lazy sequence lifetimes

- Location: `test.cpp:45` (`combine`), `test.cpp:51` (`combine`), and `test.cpp:58` (`combine`).
- Issue: If tabulation or filtering returns a lazy view that retains its callable, the reference captures can outlive local sequences after `combine` returns. Returning `res` by value would not repair references inside such a view. If these operations eagerly materialize owning sequences, this concern does not apply. Binding a returned temporary to a local `auto&&` alone is not evidence of a dangling reference; its lifetime can extend through the local scope.
- Recommendation: Verify evaluation and ownership guarantees. If results are lazy, materialize the returned result before leaving the function or make its retained dependencies own their data.

### [UNVERIFIED] Equal-x ordering across the two merges

- Location: `test.cpp:27` (`combine`), `test.cpp:37` (`combine`), and `test.cpp:48` (`combine`).
- Issue: The two merges reverse the source order, then combine scanned heights by index. Equal-x groups require compatible tie handling so the paired heights describe the same logical stage at that x. The final filter only removes consecutive equal heights; it does not consolidate distinct heights at the same x. The absent merge implementation and unspecified input normalization prevent establishing whether this can produce an incorrect result. Consistent stable merge semantics with canonical inputs may satisfy the requirement.
- Recommendation: Check the library's tie guarantees and validate coincident starts, coincident ends, a start coinciding with an end, and any permitted repeated-x inputs against a reference skyline. If those guarantees are insufficient, group events by x before emitting the combined height.

## 3. Checklist observations

- The scan operator selects the rightmost non-sentinel value. Under the intended integer interpretation it is associative and has identity `-1`, making it appropriate for a parallel scan.
- There is no explicit recursion, so no tail-recursion or CPS transformation is warranted.
- The code already expresses its main work through map, merge, scan, tabulate, and filter. No loop conversion is needed.
- No explicit mutation of inputs or shared state is present. No overflow-producing arithmetic on coordinates or heights was identified; indexing safety still depends on the library operations preserving lengths and the stated input contracts.
- No third-party implementation was reviewed or refactored.
