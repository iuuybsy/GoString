#ifndef GO_STRING_H
#define GO_STRING_H

#include "go_color_enum.h"
#include "go_point.h"

#include <unordered_set>
#include <functional>

namespace go_logic {
    class GoString {
    public:
        Stone color;
        std::unordered_set<Point> stones;
        std::unordered_set<Point> liberties;

    public:
        GoString(Stone color, const std::unordered_set<Point>& stones,
                 const std::unordered_set<Point>& liberties);
        ~GoString();

        void add_liberty(const Point& point) noexcept;
        void remove_liberty(const Point& point) noexcept;

        bool operator==(const GoString& other) const;
    };
}

namespace std {
    template <>
    struct hash<go_logic::GoString> {
        std::size_t operator()(const go_logic::GoString& go_string) const {
            std::size_t color_hash = std::hash<int>()(static_cast<int>(go_string.color));
            std::size_t stones_hash = 0;
            std::size_t liberties_hash = 0;

            for (const auto& point : go_string.stones) {
                auto point_hash = std::hash<int>()(point.x) ^ (std::hash<int>()(point.y) << 1);
                stones_hash ^= point_hash + 0x9e3779b9 + (stones_hash << 6) + (stones_hash >> 2);
            }

            // 计算 liberties 集合的哈希值
            for (const auto& point : go_string.liberties) {
                auto point_hash = std::hash<int>()(point.x) ^ (std::hash<int>()(point.y) << 1);
                liberties_hash ^= point_hash + 0x9e3779b9 + (liberties_hash << 6) + (liberties_hash >> 2);
            }

            // 组合所有哈希值
            return color_hash ^ (stones_hash << 1) ^ (liberties_hash << 2);
        }
    };
}

#endif //GO_STRING_H
