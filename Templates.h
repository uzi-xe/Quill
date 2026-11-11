#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <vector>
#include <string>
#include <sstream>
#include <utility>

template <typename T>
T clampValue(T val, T low, T high) {
    return (val < low) ? low : (val > high ? high : val);
}

template <typename T>
std::string joinItems(const std::vector<T>& items, const std::string& sep = ", ") {
    std::ostringstream oss;
    for (std::size_t i = 0; i < items.size(); ++i) {
        if (i > 0) oss << sep;
        oss << items[i];
    }
    return oss.str();
}

template <typename K, typename V>
class Registry {
private:
    std::vector<std::pair<K, V>> entries;

public:
    void set(const K& key, const V& value) {
        for (auto& pair : entries) {
            if (pair.first == key) {
                pair.second = value;
                return;
            }
        }
        entries.push_back({key, value});
    }

    V getOrDefault(const K& key, const V& fallback) const {
        for (const auto& pair : entries) {
            if (pair.first == key) return pair.second;
        }
        return fallback;
    }

    std::size_t size() const { return entries.size(); }
};

#endif