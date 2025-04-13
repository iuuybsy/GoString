#include "../headers/go_string.h"

namespace go_logic {
    GoString::GoString(Stone color, const std::unordered_set<Point>& stones,
                       const std::unordered_set<Point>& liberties):
                       color(color), stones(stones), liberties(liberties) {}

    GoString::~GoString() = default;

    void GoString::add_liberty(const Point& point) noexcept {
        this->liberties.insert(point);
    }

    void GoString::remove_liberty(const Point& point) noexcept {
        this->liberties.erase(point);
    }

    bool GoString::operator==(const GoString& other) const {
        return color == other.color && stones == other.stones && liberties == other.liberties;
    }
}