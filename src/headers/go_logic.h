#ifndef GO_LOGIC_H
#define GO_LOGIC_H

#include "go_point.h"
#include "go_color_enum.h"
#include "go_string.h"

#include <unordered_map>
#include <vector>
#include <deque>
#include <string>

namespace go_logic {
    const int BOARD_SIZE = 19;

    enum class MoveType {
        Place,
        Pass,
        Regret,
    };

    struct NeighborInfo {
        std::vector<Point> free;
        std::vector<Point> friendly;
        std::vector<Point> hostile;
    };

    class GoLogic {
    private:
        std::unordered_map<Point, GoString> board;
        std::deque<std::string> ko_record;
        std::vector<std::pair<std::unordered_map<Point, GoString>, bool>> history;
        bool is_black_move;
    public:
        GoLogic();
        ~GoLogic();

        void move(MoveType move_type, int x, int y);
        void move(MoveType move_type);
    private:
        static bool is_valid_cord(int x, int y);
        static GoString merge_strings(const GoString& string1, const GoString& string2);

        NeighborInfo get_neighbor_info(const Point& point) const noexcept;
        void print_board_info() const noexcept;
        void place_stone(const Point& point);
        void pass_move();
        void regret();
    };
}

#endif //GO_LOGIC_H
