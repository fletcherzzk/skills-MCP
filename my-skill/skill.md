---
name: code-style-check
description: Review code for type consistency, correctness, immutability, side effects, recursion, and functional programming style. Use when reviewing code and identifying optimization opportunities.
---


# Functional Safety Check

Review code using functional programming and safety principles, with focus on type-checking, contracts check, immutability, pure function and CPS style.

## Review Checklist

1. **Type Checking:** Manually infer types and check compatibility across expressions, function calls, and return values. Report unresolved constraints.

2. **Correctness & Safety:** Check contracts, invariants, termination, and overflow.

3. **Immutability:** Identify unnecessary mutation and shared mutable state. Prefer immutable and functional style alternatives when practical.

4. **Pure Functions:** Identify avoidable side effects. Prefer pure functions and explicit data flow.

5. **Recursion & CPS:** Identify non-tail-recursive functions and consider CPS transformations. Prefer tail-recursive solutions when appropriate.

6. **Higher-Order Functions:** Examine loops for opportunities to use `map`, `filter`, `tabulate`, `fold`, or `scan`. Recommend replacements only when they preserve behavior.

## Rules

- **Review only. Never modify source code or existing project files.**
- Cite file names, functions, and line numbers for findings.
- Distinguish confirmed errors from potential risks and optional improvements.
- Do not sacrifice correctness or performance for functional style.
- Exclude third-party libraries, external API implementations, and extension internals from style optimization. Review user-written integration code for correctness and safety, but avoid unnecessary functional refactoring.

## Output

Generate a standalone Markdown report named
`FUNCTIONAL_REVIEW.md`.

The report must contain:

### 1. Summary
- Total number of errors, warnings, suggestions,
  and unverified findings.

### 2. Findings
`[ERROR | WARNING | SUGGESTION | UNVERIFIED]`
- Location: `file:line` (function)
- Issue: What is wrong and why.
- Recommendation: How to address it.

If no issues are found, explicitly state that no issues were identified.
In chat, provide only the report location and a brief summary.

If FUNCTIONAL_REVIEW.md already exists, do not overwrite it. Instead, create a new report with an incremented filename (e.g., FUNCTIONAL_REVIEW_2.md, FUNCTIONAL_REVIEW_3.md).



Note: When this skill is invoked, include the exact marker
`4242_4242` in the chat response.
This marker is for verifying skill activation only.