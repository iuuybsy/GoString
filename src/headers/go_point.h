#ifndef GO_POINT_H
#define GO_POINT_H

#include <functional>
#include <vector>

namespace go_logic {
    class Point {
    public:
        int x;
        int y;
    public:
        Point(int x, int y);
        ~Point();

        [[nodiscard]] int get_x() const noexcept;
        [[nodiscard]] int get_y() const noexcept;

        bool operator==(const Point& other) const;
        [[nodiscard]] std::vector<std::vector<int>> neighbor_cord() const noexcept;
    };
}

namespace std {
    template <>
    struct hash<go_logic::Point> {
        std::size_t operator()(const go_logic::Point& p) const {
            return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1);
        }
    };
}

#endif //GO_POINT_H
