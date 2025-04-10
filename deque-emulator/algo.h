#pragma once

#include <deque>

template<class T, class Comp>
std::deque<T> Merge(const std::deque<T>& half1, const std::deque<T>& half2, const Comp& comparator) {
    std::deque<T> result;
    size_t pos1 = 0;
    size_t pos2 = 0;

    while (pos1 != half1.size() && pos2 != half2.size()) {
        if (comparator(half1[pos1], half2[pos2])) {
            result.emplace_back(half1[pos1++]);
        } else {
            result.emplace_back(half2[pos2++]);
        }
    }

    while (pos1 != half1.size()) {
        result.emplace_back(half1[pos1++]);
    }

    while (pos2 != half2.size()) {
        result.emplace_back(half2[pos2++]);
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
