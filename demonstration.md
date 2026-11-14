default-constructible/assignable result; thread-safe pure callable",
        "input_modified": false,
        "work": "O(n)"
      },
      "source_code": "#pragma once\n\n#include <algorithm>\n#include <execution>\n#include <functional>\n#include <type_traits>\n#include <utility>\n#include <vector>\n\nnamespace functional_cpp {\n\n// O(n) work; requests parallel execution through the C++17 execution policy.\n// Does not modify xs. f must be pure and safe to invoke concurrently.\n// Limitation: result values must be default-constructible and assignable.\ntemplate <class T, class F>\nauto parallel_map(const std::vector<T>& xs, F f)\n    -> std::vector<std::decay_t<std::invoke_result_t<F&, const T&>>> {\n    using R = std::decay_t<std::invoke_result_t<F&, const T&>>;\n    std::vector<R> result(xs.size());\n    std::transform(std::execution::par, xs.begin(), xs.end(), result.begin(), f);\n    return result;\n}\n\n} // namespace functional_cpp\n"
    }
  ]
}
// MCP successfully registered.