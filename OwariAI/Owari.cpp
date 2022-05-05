#include "ABPrunedMinimax.cpp"

class Owari {
public:
    int board[14] = { 3,3,3,3,3,3,0,3,3,3,3,3,3,0 };
    PlayerAlgo* cpu;
    enum class Turn { SOUTH, NORTH };
    Turn firstTurn = Turn::SOUTH;
    enum class Winner { NONE, SOUTH, NORTH, TIE };
    Winner winner = Winner::NONE;
    Owari(PlayerAlgo* cpu_in) {
        cpu = cpu_in;
        std::cout << "Is the player 1st or 2nd (1 or 2): ";
        int choice = 0;
        while (choice != 1 && choice != 2) {
            std::cout << "Choose 1 or 2: ";
            std::cin >> choice;
        }
        if (choice == 1) {
            firstTurn = Turn::SOUTH;
        }
        else {
            firstTurn = Turn::NORTH;
        }

    }

    int acrossMap[14] = { 12, 11, 10, 9, 8, 7, -1, 5, 4, 3, 2, 1, 0, -1 };

    void checkWin() {
        //Check to see if any side has run out of moves
        int southTotal = 0;
        int northTotal = 0;
        for (int i = 0; i < 6; i++) {
            southTotal += board[i];
        }
        for (int i = 7; i < 13; i++) {
            northTotal += board[i];
        }

        //if one side ran out, add the remaining to the opposing goal
        if (southTotal == 0) {
            board[13] += northTotal;
        }
        else if (northTotal == 0) {
            board[6] += southTotal;
        }

        //declare a winner if someone ran out
        if (southTotal == 0 || northTotal == 0) {
            if (board[6] > board[13]) {
                winner = Winner::SOUTH;
            }
            else if (board[13] > board[6]) {
                winner = Winner::NORTH;
            }
            else {
                winner = Winner::TIE;
            }
            return;
        }
    }

    void printBoard() {
        std::cout << "---" << board[12] << "-" << board[11] << "-" << board[10] << "-" << board[9] << "-" << board[8] << "-" << board[7] << "---" << "\n";
        std::cout << board[13] << "---------------" << board[6] << "\n";
        std::cout << "---" << board[0] << "-" << board[1] << "-" << board[2] << "-" << board[3] << "-" << board[4] << "-" << board[5] << "---" << "\n\n";
    }

    void printArbitraryBoard(int* board) {
        std::cout << "---" << board[12] << "-" << board[11] << "-" << board[10] << "-" << board[9] << "-" << board[8] << "-" << board[7] << "---" << "\n";
        std::cout << board[13] << "---------------" << board[6] << "\n";
        std::cout << "---" << board[0] << "-" << board[1] << "-" << board[2] << "-" << board[3] << "-" << board[4] << "-" << board[5] << "---" << "\n\n";
    }

    int getUserMove() {
        int choice = -1;
        while (choice < 0 || choice > 5) {
            std::cout << "Choose a pit (1-6): ";
            std::cin >> choice;
            choice -= 1;
        }
        return choice;
    }

    void doUserTurn() {
        int pitPointer = getUserMove();
        std::cout << "User moved pit " << pitPointer << "!\n";
        int numToDistribute = board[pitPointer];
        while (numToDistribute == 0) {
            std::cout << "That pit has no seeds! (try again)\n";
            pitPointer = getUserMove();
            numToDistribute = board[pitPointer];
        }

        board[pitPointer] = 0;//empty original pit
        //distribute seeds in next pits
        for (int i = numToDistribute; i > 0; i--) {
            pitPointer++;
            //handle a full loop
            if (pitPointer == 13) {
                pitPointer = 0;
            }
            board[pitPointer] += 1;
        }
        //check if last seed placed in empty pit on south side. If so, capture adjacent pit.
        if (board[pitPointer] == 1 && pitPointer < 6) {
            board[6] += board[acrossMap[pitPointer]];
            board[acrossMap[pitPointer]] = 0;
        }

        printBoard();
    }

    void doCPUTurn() {
        int pitPointer = cpu->getMove(board);
        std::cout << "CPU moved pit " << pitPointer << "(" << pitPointer - 6 - 1 << ")!\n";// <<<======== OFFSET PRINTED MOVE HERE
        int numToDistribute = board[pitPointer];
        while (numToDistribute == 0) {
            //std::cout << "CPU chose an empty pit: " << pitPointer << "\n";
            pitPointer = cpu->getMove(board);
            numToDistribute = board[pitPointer];
        }

        board[pitPointer] = 0;//empty original pit
        //distribute seeds in next pits
        for (int i = numToDistribute; i > 0; i--) {
            pitPointer++;
            //handle a full loop
            if (pitPointer == 14) {
                pitPointer = 0;
            }
            else if (pitPointer == 6) {
                pitPointer = 7;
            }
            board[pitPointer] += 1;
        }
        //check if last seed placed in empty pit on south side. If so, capture adjacent pit.
        if (board[pitPointer] == 1 && pitPointer < 13 && pitPointer > 6) {
            board[13] += board[acrossMap[pitPointer]];
            board[acrossMap[pitPointer]] = 0;
        }

        printBoard();
    }

    void doRound() {
        int whoWon;
        if (firstTurn == Turn::SOUTH) {
            doUserTurn();
            checkWin();
            if (winner != Winner::NONE) return;
            doCPUTurn();
            checkWin();
        }
        else {
            doCPUTurn();
            checkWin();
            if (winner != Winner::NONE) return;
            doUserTurn();
            checkWin();
        }
    }
};