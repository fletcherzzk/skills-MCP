#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

struct Student {
    std::string name;
    int homework;
    int exam;
};

struct RankedStudent {
    std::string name;
    double score;
};

// Final grade: 40% homework + 60% exam.
double final_score(const Student& student) {
    // Widen before multiplication; round the exact weighted total only once.
    static_assert(std::numeric_limits<int>::digits <= 31,
                  "Score calculation supports integers with up to 31 value bits.");
    const std::int64_t weighted_total =
        2 * static_cast<std::int64_t>(student.homework) +
        3 * static_cast<std::int64_t>(student.exam);
    return static_cast<double>(weighted_total) / 5.0;
}

// Rank students from highest to lowest final score.
// Students with equal scores retain their original order.
// The input student list remains unchanged.
std::vector<RankedStudent> build_ranking(
    const std::vector<Student>& students
) {
    std::vector<RankedStudent> ranking;
    ranking.reserve(students.size());

    for (const Student& student : students) {
        ranking.push_back({student.name, final_score(student)});
    }

    std::stable_sort(ranking.begin(), ranking.end(),
                     [](const RankedStudent& a, const RankedStudent& b) {
                         return a.score > b.score;
                     });

    return ranking;
}

// Calculate the average final score, preserving fractional and negative scores.
// An empty input produces 0.0.
double average_score(const std::vector<Student>& students) {
    if (students.empty()) {
        return 0.0;
    }

    double total = 0.0;
    for (const Student& student : students) {
        total += final_score(student);
    }

    return total / static_cast<double>(students.size());
}

// Count students whose final score is at least threshold.
std::vector<Student>::size_type count_passing(
    const std::vector<Student>& students,
    double threshold
) {
    std::vector<Student>::size_type count = 0;

    for (const Student& student : students) {
        if (final_score(student) >= threshold) {
            ++count;
        }
    }

    return count;
}

int main() {
    const std::vector<Student> students = {
        {"Alice", 80, 90},
        {"Bob", 95, 75},
        {"Cara", 70, 100},
        {"Dan", 80, 90}
    };

    const auto ranking = build_ranking(students);

    std::cout << "Ranking:\n";
    for (const auto& entry : ranking) {
        std::cout << entry.name << ": "
                  << entry.score << "\n";
    }

    std::cout << "Average: "
              << average_score(students) << "\n";

    std::cout << "Passing (>= 85): "
              << count_passing(students, 85.0) << "\n";

    return 0;
}
