#include <algorithm>
#include "PlayerAlgo.cpp"

class RandomChoice : public PlayerAlgo {
public:
    int getMove(int* board) {
        return rand() % 6 + 7;
    }
};