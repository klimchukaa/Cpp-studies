#include "admission.h"
#include <tuple>
#include <algorithm>

bool CmpByScore(const Applicant* a, const Applicant* b) {
    return std::tie(b->points, a->student.birth_date.year, a->student.birth_date.month, a->student.birth_date.day,
                    a->student.name) < std::tie(a->points, b->student.birth_date.year, b->student.birth_date.month,
                                                b->student.birth_date.day, b->student.name);
}

bool CmpByName(const Student* a, const Student* b) {
    return std::tie(a->name, a->birth_date.year, a->birth_date.month, a->birth_date.day) <
           std::tie(b->name, b->birth_date.year, b->birth_date.month, b->birth_date.day);
}

AdmissionTable FillUniversities(const std::vector<University>& universities, const std::vector<Applicant>& applicants) {
    std::vector<const Applicant*> applicants_sorted;
    for (size_t i = 0; i < applicants.size(); ++i) {
        applicants_sorted.push_back(&applicants[i]);
    }
    std::sort(applicants_sorted.begin(), applicants_sorted.end(), CmpByScore);
    AdmissionTable admission_table;
    std::unordered_map<std::string, size_t> available_places;
    for (size_t i = 0; i < universities.size(); ++i) {
        available_places[universities[i].name] = universities[i].max_students;
    }
    for (size_t i = 0; i < applicants_sorted.size(); ++i) {
        for (size_t j = 0; j < applicants_sorted[i]->wish_list.size(); ++j) {
            if (available_places[applicants_sorted[i]->wish_list[j]] > 0) {
                admission_table[applicants_sorted[i]->wish_list[j]].push_back(&applicants_sorted[i]->student);
                --available_places[applicants_sorted[i]->wish_list[j]];
                break;
            }
        }
    }
    for (size_t i = 0; i < universities.size(); ++i) {
        std::sort(admission_table[universities[i].name].begin(), admission_table[universities[i].name].end(),
                  CmpByName);
    }
    return admission_table;
}
