#include "../headers/go_point.h"

namespace go_logic {
    Point::Point(int x, int y): x(x), y(y) {}
    Point::~Point() = default;

    int Point::get_x() const noexcept {
        return x;
    }

    int Point::get_y() const noexcept {
        return y;
    }

    bool Point::operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    std::vector<std::vector<int>> Point::neighbor_cord() const noexcept {
        return {
            {x + 1, y},
            {x - 1, y},
            {x, y + 1},
            {x, y - 1}
        };
    }
}