
#include <algorithm>
#include <iostream>
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
    return student.homework * 0.4 + student.exam * 0.6;
}

// Rank students from highest to lowest final score.
// Students with equal scores should retain their original order.
// The input student list should remain unchanged.
std::vector<RankedStudent> build_ranking(
    std::vector<Student>& students
) {
    std::sort(students.begin(), students.end(),
              [](const Student& a, const Student& b) {
                  return final_score(a) > final_score(b);
              });

    std::vector<RankedStudent> ranking;

    for (std::size_t i = 0; i < students.size(); ++i) {
        RankedStudent entry;
        entry.name = students[i].name;
        entry.score = final_score(students[i]);
        ranking.push_back(entry);
    }

    return ranking;
}

// Calculate the average final score.
// An empty input should produce 0.0.
double average_score(const std::vector<Student>& students) {
    int total = 0;

    for (const Student& student : students) {
        total += static_cast<int>(final_score(student));
    }

    return total / students.size();
}

// Count students whose final score is at least threshold.
int count_passing(
    const std::vector<Student>& students,
    double threshold
) {
    int count = 0;

    for (std::size_t i = 0; i < students.size(); ++i) {
        if (final_score(students[i]) >= threshold) {
            count++;
        }
    }

    return count;
}

int main() {
    std::vector<Student> students = {
        {"Alice", 80, 90},
        {"Bob", 95, 75},
        {"Cara", 70, 100},
        {"Dan", 80, 90}
    };

    auto ranking = build_ranking(students);

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
