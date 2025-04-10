#pragma once

#include <algorithm>
#include <deque>
#include <random>
#include <string>
#include <unordered_map>

#include "algo.h"


class Model {
    public:
        using ValueType = std::string;
        using Iterator = std::deque<ValueType>::iterator;
        using ConstIterator = std::deque<ValueType>::const_iterator;

        size_t Size() const {
            return items_.size();
        }

        size_t GetCurrentIteratorPos() const {
            return std::distance(items_.cbegin(), ConstIterator(iterator_));
        }

        ValueType GetContentByIterator() const {
            return IsIteratorEnd() ? ValueType{} : *iterator_;
        }

        void SetIteratorBegin() {
            iterator_ = items_.begin();
        }

        void SetIteratorEnd() {
            iterator_ = items_.end();
        }

        void MoveIterator(size_t offset) {
            if (offset > Size()) {
                return;
            }
            iterator_ = std::next(items_.begin(), offset);
        }

        bool IsIteratorBegin() const {
            return iterator_ == items_.begin();
        }

        bool IsIteratorEnd() const {
            return iterator_ == items_.end();
        }

        void Increment() {
            if (iterator_ == items_.end()) {
                return;
            }
            ++iterator_;
        }

        void Decrement() {
            if (iterator_ == items_.begin()) {
                return;
            }
            --iterator_;
        }

        void PopBack() {
            if (items_.empty()) {
                return;
            }
            items_.pop_back();
            SetIteratorBegin();
        }

        void PopFront() {
            if (items_.empty()) {
                return;
            }
            items_.pop_front();
            SetIteratorBegin();
        }

        void PushBack(const ValueType& str) {
            items_.push_back(str);
            SetIteratorBegin();
        }

        void PushFront(const ValueType& str) {
            items_.push_front(str);
            SetIteratorBegin();
        }

        void Edit(const ValueType& value) {
            if (iterator_ == items_.end()) {
                return;
            }
            *iterator_ = value;
        }

        void Erase() {
            items_.erase(iterator_);
            SetIteratorBegin();
        }

        void Insert(const ValueType& value) {
            items_.insert(iterator_, value);
            SetIteratorBegin();
        }

        void Clear() {
            items_.clear();
            SetIteratorBegin();
        }

        void MinElement() {
            iterator_ = std::min_element(items_.begin(), items_.end());
        }

        void MaxElement() {
            iterator_ = std::max_element(items_.begin(), items_.end());
        }

        void MergeSort() {
            items_ = ::MergeSort(items_, std::less<ValueType>());
            SetIteratorBegin();
        }
        // Изначальный вариант но он не справится с регистронезависимой сортировкой кириллических строк
        // void MergeSortCaseFree() {
        //     auto case_free_predicat = [](unsigned char lc, unsigned char rc) {
        //         return std::tolower(lc) < std::tolower(rc);
        //     };

        //     auto case_free_sort = [&](const std::string& left, const std::string& right) {
        //         bool result = std::lexicographical_compare(left.begin(), left.end(), right.begin(), right.end(), case_free_predicat);
        //         return result;
        //     };

        //     items_ = ::MergeSort(items_, case_free_sort);
        //     SetIteratorBegin();
        // }

        void MergeSortCaseFree() {
            auto case_free_compare = [](const std::string& left, const std::string& right) {
                auto left_lower = Model::ToLowerRuUtf8(left);
                auto right_lower = Model::ToLowerRuUtf8(right);
                bool result = std::lexicographical_compare(left_lower.begin(), left_lower.end(), right_lower.begin(), right_lower.end());
                return result;
            };
            items_ = ::MergeSort(items_, case_free_compare);
            SetIteratorBegin();
        }

        void Unique() {
            if (!std::is_sorted(items_.begin(), items_.end())) {
                return;
            }

            auto to_del = std::unique(items_.begin(), items_.end());
            items_.erase(to_del, items_.end());
            SetIteratorBegin();
        }

        void Reverse() {
            std::reverse(items_.begin(), items_.end());
        }

        void Shuffle(std::mt19937& gen) {
            std::shuffle(items_.begin(), items_.end(), gen);
        }

        bool LowerBound(const ValueType& value) {
            if (!std::is_sorted(items_.begin(), items_.end())) {
                return false;
            }
            iterator_ = std::lower_bound(items_.begin(), items_.end(), value);
            return true;
        }

        bool UpperBound(const ValueType& value) {
            if (!std::is_sorted(items_.begin(), items_.end())) {
                return false;
            }
            iterator_ = std::upper_bound(items_.begin(), items_.end(), value);
            return true;
        }

        void Find(const ValueType& value) {
            iterator_ = std::find(items_.begin(), items_.end(), value);
        }

        size_t Count(const ValueType& value) {
            return std::count(items_.begin(), items_.end(), value);
        }

        void UpdateData(const std::deque<ValueType>& data) {
            items_ = data;
            SetIteratorBegin();
        }

        void Resize(size_t size) {
            items_.resize(size);
            SetIteratorBegin();
        }

        const std::deque<ValueType>& GetItems() const {
            return items_;
        }

    private:
        static std::string ToLowerRuUtf8(const std::string& str) {
            static const std::unordered_map<std::string, std::string> kToLower {
                {"А", "а"}, {"Б", "б"}, {"В", "в"}, {"Г", "г"},
                {"Д", "д"}, {"Е", "е"}, {"Ё", "ё"}, {"Ж", "ж"},
                {"З", "з"}, {"И", "и"}, {"Й", "й"}, {"К", "к"},
                {"Л", "л"}, {"М", "м"}, {"Н", "н"}, {"О", "о"},
                {"П", "п"}, {"Р", "р"}, {"С", "с"}, {"Т", "т"},
                {"У", "у"}, {"Ф", "ф"}, {"Х", "х"}, {"Ц", "ц"},
                {"Ч", "ч"}, {"Ш", "ш"}, {"Щ", "щ"}, {"Ъ", "ъ"},
                {"Ы", "ы"}, {"Ь", "ь"}, {"Э", "э"}, {"Ю", "ю"},
                {"Я", "я"}
            };

            std::string result;
            result.reserve(str.size());
            size_t i = 0;
            while (i < str.size()) {
                unsigned char ch = str[i];
                // В кодировке UTF8 первый байт для кириллицы равен D0 и D1
                if (ch == 0xD0 || ch == 0xD1) {
                    std::string utf8_char = str.substr(i, 2);
                    auto it = kToLower.find(utf8_char);
                    // Если символ найден в таблице, то добавляем к результату его пару
                    // иначе символ принадлежит кириллице, но в таблице его нет (символ в нижнем регистре)
                    result += (it != kToLower.end()) ? it->second : utf8_char;
                    // т.к. обрабатывался 2-байтовый символ, позиция смещается на 2
                    i += 2;
                } else {
                    // случай обработки 1-байтового символа ASCII
                    result += std::tolower(str[i]);
                    i += 1;
                }
            }
            return result;
        }

        std::deque<ValueType> items_;
        Iterator iterator_ = items_.begin();
};
