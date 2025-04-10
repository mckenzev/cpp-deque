#pragma once

#include <deque>

template<class T, class Comp>
std::deque<T> Merge(const std::deque<T>& half1, const std::deque<T>& half2, const Comp& comparator) {
    std::deque<T> result;
    auto it_1 = half1.begin();
    auto it_2 = half2.begin();

    while (it_1 != half1.end() && it_2 != half2.end()) {
        if (comparator(*it_1, *it_2)) {
            result.emplace_back(*it_1++);
        } else {
            result.emplace_back(*it_2++);
        }
    }

    while (it_1 != half1.end()) {
        result.emplace_back(*it_1++);
    }

    while (it_2 != half2.end()) {
        result.emplace_back(*it_2++);
    }
    return result;
}

template<class T, class Comp>
std::deque<T> MergeSort(const std::deque<T>& src, const Comp& comparator) {
    if (src.size() <= 1) {
        return src;
    }

    size_t mid = src.size() / 2;

    auto it_mid =  std::next(src.begin(), mid);
    auto left = MergeSort(std::deque<T>(src.begin(), it_mid), comparator);
    auto right = MergeSort(std::deque<T>(it_mid, src.end()), comparator);

    return Merge(left, right, comparator);
}
