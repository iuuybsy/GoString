#include "../headers/go_logic.h"

#include <iostream>
#include <string>

namespace go_logic {
    GoLogic::GoLogic(): is_black_move(true) {}

    GoLogic::~GoLogic() = default;

    void GoLogic::place_stone(const Point& point) {
        if (board.count(point)) {
            std::cout << "This point is already occupied" << std::endl;
            return;
        }

        if (is_black_move) {
            std::cout << "Black move" << std::endl;
        }
        else {
            std::cout << "White move" << std::endl;
        }

        auto color = is_black_move ? Stone::BLACK: Stone::WHITE;
        std::unordered_set<Point> stones = {point};
        std::unordered_set<Point> liberties = {};
        NeighborInfo neighbor_info = get_neighbor_info(point);

        std::cout << "Neighbor info get" << std::endl;

        for (const auto& free_point: neighbor_info.free) {
            liberties.insert(free_point);
        }
        auto target_string = GoString(color, stones, liberties);

        std::cout << "New string set." << std::endl;

        std::unordered_set<Point> unified_strings;
        std::unordered_set<Point> removed_strings;
        for (auto & it : board) {
            if (it.second.liberties.count(point)) {
                if (it.second.color == color) {
                    target_string = merge_strings(target_string, it.second);
                    unified_strings.insert(it.first);
                }
                else {
                    it.second.remove_liberty(point);
                    if (it.second.liberties.empty()) {
                        removed_strings.insert(it.first);
                    }
                }
            }
        }

        board.insert({point, target_string});

        for (const auto& unified_string: unified_strings) {
            auto& unified_string_info = board.at(unified_string);
            std::cout << "Unified stones: " << std::endl;
            for (const auto& stone: unified_string_info.stones) {
                std::cout << "(" << stone.x << ", " << stone.y << ")" << std::endl;
            }
            board.erase(unified_string);
        }

        for (const auto& removed_string: removed_strings) {
            auto& removed_string_info = board.at(removed_string);
            std::cout << "Removed stones: " << std::endl;
            for (const auto& stone: removed_string_info.stones) {
                std::cout << "(" << stone.x << ", " << stone.y << ")" << std::endl;
            }
            removed_string_info.remove_liberty(point);
            if (removed_string_info.liberties.empty()) {
                board.erase(removed_string);
            }
        }
        board.emplace(point, target_string);

        std::vector<std::string> board_info(BOARD_SIZE, std::string(BOARD_SIZE, '.'));
        for (const auto& it : board) {
            auto color_char = it.second.color == Stone::BLACK? 'X': 'O';

            const auto& stones = it.second.stones;
            for (const auto& stone: stones) {
                board_info[stone.x][stone.y] = color_char;
            }
        }

        std::string board_info_str = board_info[0];
        for (int i = 1; i < BOARD_SIZE; i++) {
            board_info_str += board_info[i];
        }
        if (ko_record.size() == 2) {
            if (board_info_str == ko_record[0]) {
                this->board = history.back().first;
                this->is_black_move = history.back().second;
                return;
            }
            else {
                ko_record.pop_front();
            }
        }
        ko_record.push_back(board_info_str);

        is_black_move = !is_black_move;
        print_board_info();
        std::unordered_map<Point, GoString> cur_board = board;
        bool cur_turn = is_black_move;
        history.push_back({cur_board, cur_turn});
    }

    void GoLogic::pass_move() {
        print_board_info();
        is_black_move = !is_black_move;
        std::unordered_map<Point, GoString> cur_board = board;
        bool cur_turn = is_black_move;
        history.push_back({cur_board, cur_turn});
    }

    void GoLogic::regret() {
        if (history.empty()) {
            print_board_info();
            return;
        }
        history.pop_back();
        this->board = history.back().first;
        this->is_black_move = history.back().second;
        print_board_info();
    }

    void GoLogic::move(MoveType move_type, int x, int y) {
        if (move_type != MoveType::Place) {
            throw std::invalid_argument("Only place move is supported");
        }
        if (!is_valid_cord(x, y)) {
            throw std::invalid_argument("Invalid coordinates");
        }
        place_stone(Point(x, y));
    }

    void GoLogic::move(MoveType move_type) {
        if (move_type == MoveType::Place) {
            throw std::invalid_argument("Place move requires coordinates");
        }
        else if (move_type == MoveType::Pass) {
            this->pass_move();
        }
        else if (move_type == MoveType::Regret) {
            this->regret();
        }
        else {
            throw std::invalid_argument("Invalid move type");
        }
    }

    bool GoLogic::is_valid_cord(int x, int y) {
        return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
    }

    GoString GoLogic::merge_strings(const GoString& string1, const GoString& string2) {
        if (string1.color != string2.color) {
            throw std::invalid_argument("Strings must be of the same color");
        }

        std::unordered_set<Point> new_stones = string1.stones;
        new_stones.insert(string2.stones.begin(), string2.stones.end());
        std::unordered_set<Point> new_liberties = string1.liberties;
        new_liberties.insert(string2.liberties.begin(), string2.liberties.end());
        for (const auto& point: new_stones) {
            new_liberties.erase(point);
        }

        GoString result(string1.color, new_stones, new_liberties);
        return result;
    }

    NeighborInfo GoLogic::get_neighbor_info(const Point& point) const noexcept {
        NeighborInfo neighbor_info;
        auto neighbor_cords = point.neighbor_cord();
        for (const auto& cord: neighbor_cords) {
            if (!is_valid_cord(cord[0], cord[1])) {
                continue;
            }
            auto neighbor_point = Point(cord[0], cord[1]);
            if (board.count(neighbor_point) and board.count(point)) {
                if (board.at(neighbor_point).color == board.at(point).color) {
                    neighbor_info.friendly.push_back({cord[0], cord[1]});
                }
                else {
                    neighbor_info.hostile.push_back({cord[0], cord[1]});
                }
            }
            else {
                neighbor_info.free.push_back({cord[0], cord[1]});
            }
        }
        return neighbor_info;
    }

    void GoLogic::print_board_info() const noexcept {
        std::vector<std::string> board_info(BOARD_SIZE, std::string(BOARD_SIZE, '.'));
        for (const auto& it : board) {
            auto color = it.second.color == Stone::BLACK? 'X': 'O';

            const auto& stones = it.second.stones;
            for (const auto& stone: stones) {
                board_info[stone.x][stone.y] = color;
            }
        }
        for (int i = BOARD_SIZE - 1; i >= 0; --i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                std::cout << board_info[j][i] << " ";
            }
            std::cout << std::endl;
        }
    }
}
