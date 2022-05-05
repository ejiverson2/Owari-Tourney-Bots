#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include "PlayerAlgo.cpp"

class ABPrunedMinimax : public PlayerAlgo {
public:
    int nodesLookedAt = 0;
    int acrossMap[14] = { 12, 11, 10, 9, 8, 7, -1, 5, 4, 3, 2, 1, 0, -1 };

    int* deepCopy(int* board) {
        int* newBoard = new int[14];
        for (int i = 0; i < 14; i++) {
            newBoard[i] = board[i];
        }
        return newBoard;
    }

    int* getNextBoard(int* oldBoard, int move) {
        //get a new board to return
        int* board = deepCopy(oldBoard);

        int pitPointer = move;
        int numToDistribute = board[pitPointer];

        board[pitPointer] = 0;//empty original pit
        //distribute seeds in next pits
        for (int i = numToDistribute; i > 0; i--) {
            pitPointer++;
            //handle a full loop
            if (move < 6) {
                if (pitPointer == 13) {
                    pitPointer = 0;
                }
            }
            else if (move > 6 && move < 13) {
                if (pitPointer == 6) {
                    pitPointer = 7;
                }
                else if (pitPointer == 14) {
                    pitPointer = 0;
                }
            }

            board[pitPointer] += 1;
        }
        //check if last seed placed in empty pit on south side. If so, capture adjacent pit.
        if (move < 6) {
            if (board[pitPointer] == 1 && pitPointer < 6) {
                board[6] += board[acrossMap[pitPointer]];
                board[acrossMap[pitPointer]] = 0;
            }
        }
        else if (move < 13 && move > 6) {
            if (board[pitPointer] == 1 && pitPointer < 13 && pitPointer > 6) {
                board[13] += board[acrossMap[pitPointer]];
                board[acrossMap[pitPointer]] = 0;
            }
        }

        return board;
    }

    int staticEvaluation(int* board) {
        //return board[13] - board[6];
        int goalMultiplyer = 100;

        int southTotal = 0;
        int northTotal = 0;
        for (int i = 0; i < 6; i++) {
            southTotal += board[i];
        }
        for (int i = 7; i < 13; i++) {
            northTotal += board[i];
        }

        if (southTotal == 0) {
            return goalMultiplyer * (board[13] - board[6] + northTotal);
        }
        else if (northTotal == 0) {
            return goalMultiplyer * (board[13] - board[6] - southTotal);
        }

        int f = 0;
        f += (northTotal - southTotal);
        f += /*2 **/ (board[13] - board[6]);
        return f;
    }

    int alphabeta(int* board, int depth, int a, int b, bool isMax, clock_t timeout) {
        if (clock() >= timeout) {
            throw std::runtime_error("Timed out");
        }
        nodesLookedAt += 1;
        //std::cout << a << " " << b << '\n';

        //check if leaf node
        int southTotal = 0;
        int northTotal = 0;
        for (int i = 0; i < 6; i++) {
            southTotal += board[i];
        }
        for (int i = 7; i < 13; i++) {
            northTotal += board[i];
        }
        if (depth == 0 || southTotal == 0 || northTotal == 0) {
            return staticEvaluation(board);
        }

        //calculate if new values depending on if max or min
        if (isMax) {
            int value = -999999;
            for (int i = 7; i < 13; i++) {
                if (board[i] == 0) continue;
                int* nextBoard = getNextBoard(board, i);
                value = std::max(value, alphabeta(nextBoard, depth - 1, a, b, false, timeout));
                delete[] nextBoard;
                if (value >= b) {
                    break;
                }
                a = std::max(a, value);
            }
            //std::cout << value << '\n';
            return value;
        }
        else {
            int value = 999999;
            for (int i = 0; i < 6; i++) {
                if (board[i] == 0) continue;
                int* nextBoard = getNextBoard(board, i);
                value = std::min(value, alphabeta(nextBoard, depth - 1, a, b, true, timeout));
                delete[] nextBoard;
                if (value <= a) {
                    break;
                }
                b = std::min(b, value);
            }
            //std::cout << value << '\n';
            return value;
        }
    }

    int getMove(int* board) {
        //Set max clock cycles allowed (maximum time it is allowed to process)
        clock_t maxT = clock() + 30 * CLOCKS_PER_SEC;
        clock_t startTime = clock();
        //Set aspiration window size
        int windowSize = 3;
        //init best overall varaibles for all minimax trees
        int overallMove;
        int overallBest;
        int lastLayerNodesLookedAt;
        int depth = 0;
        int alpha = -999999;
        int beta = 999999;
        while (true) {
            //init local varibles for each individual minimax tree
            int move = 7;
            int best = -1000000;
            int value = -1000000;
            for (int i = 7; i < 13; i++) {
                if (board[i] == 0) continue;
                try {
                    value = alphabeta(getNextBoard(board, i), depth, alpha, beta, false, maxT);
                }
                catch (std::runtime_error& e) {
                    //if a timeout occurs, return the best overall move for all the trees
                    std::cout << "Scanned " << lastLayerNodesLookedAt << " nodes.\n";
                    nodesLookedAt = 0;
                    std::cout << "Eval of best outcome: " << overallBest << '\n';
                    std::cout << "Depth " << depth - 1 << '\n';
                    return overallMove;
                }
                if (value > best) {
                    best = value;
                    move = i;
                }
            }
            //check if the final value is in ab bounds, if not, run it again;
            if (best <= alpha || best >= beta) {
                alpha = -999999;
                beta = 999999;
                continue;
            }
            //create window for next iteration
            alpha = best - windowSize;
            beta = best + windowSize;
            //update overall stats for all trees
            lastLayerNodesLookedAt = nodesLookedAt;
            nodesLookedAt = 0;
            overallMove = move;
            overallBest = best;
            std::cout << "Time Elapsed at depth " << depth << ": " << (clock() - startTime) / CLOCKS_PER_SEC << " Val: " << overallBest << '\n';
            //increment depth for next run
            depth++;
            //if the end of the game is found, the game is over, there is no point in continuing to calculate 
            if (overallBest > 100 || overallBest < -100 || depth > 30) {
                std::cout << "Scanned " << lastLayerNodesLookedAt << " nodes.\n";
                nodesLookedAt = 0;
                std::cout << "Eval of best outcome: " << overallBest << '\n';
                std::cout << "Depth " << depth - 1 << '\n';
                return overallMove;
            }
        }
    }
};