#include "src/headers/go_logic.h"

#include <iostream>

int main() {
    go_logic::GoLogic logic;
    std::cout << std::endl << "Step 1" << std::endl;
    logic.move(go_logic::MoveType::Place, 0, 0);

    std::cout << std::endl << "Step 2" << std::endl;
    logic.move(go_logic::MoveType::Place, 1, 0);

    std::cout << std::endl << "Step 3" << std::endl;
    logic.move(go_logic::MoveType::Place, 0, 1);

    std::cout << std::endl << "Step 4" << std::endl;
    logic.move(go_logic::MoveType::Place, 1, 1);

    std::cout << std::endl << "Step 5" << std::endl;
    logic.move(go_logic::MoveType::Place, 1, 2);

    std::cout << std::endl << "Step 5" << std::endl;
    logic.move(go_logic::MoveType::Place, 0, 2);

    std::cout << std::endl << "Step 6" << std::endl;
    logic.move(go_logic::MoveType::Regret);
}
