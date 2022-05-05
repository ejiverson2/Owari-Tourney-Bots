#include "Owari.cpp"

int main() {
    srand(time(NULL));
    //PlayerAlgo* cpu = new RandomChoice();
    ABPrunedMinimax* cpu = new ABPrunedMinimax();
    Owari game = Owari(cpu);

    // int r[14] = { 1,0,0,0,0,0,0,0,0,0,0,0,9,0 };
    // game.printArbitraryBoard(cpu->getNextBoard(r, 12));
    game.printBoard();
    while (game.winner == Owari::Winner::NONE) {
        game.doRound();
    }
    if (game.winner == Owari::Winner::SOUTH) {
        std::cout << "South Wins!!!\n";
    }
    else if (game.winner == Owari::Winner::NORTH) {
        std::cout << "North Wins!!!\n";
    }
    else {
        std::cout << "Tie Game, No Winners!\n";
    }
    game.printBoard();


    return 0;
}
