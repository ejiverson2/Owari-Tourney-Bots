module.exports = class Owari {
    constructor() {
        this.turn = 0; //0 = south, 1 = north
        this.board = new Array(3, 3, 3, 3, 3, 3, 0, 3, 3, 3, 3, 3, 3, 0);
        this.acrossMap = new Array(12, 11, 10, 9, 8, 7, -1, 5, 4, 3, 2, 1, 0, -1);
    }

    printBoard() {
        console.log(" ", this.board[12], this.board[11], this.board[10], this.board[9], this.board[8], this.board[7]);
        console.log(this.board[13], "           ", this.board[6]);
        console.log(" ", this.board[0], this.board[1], this.board[2], this.board[3], this.board[4], this.board[5]);
    }

    // boardToString() {
    //     return "  " + this.board[12].toString() + " " + this.board[11].toString() + " " + this.board[10].toString() + " "
    //         + this.board[9].toString() + " " + this.board[8].toString() + " " + this.board[7] + "\n"
    //         + this.board[13].toString() + "             " + "     " + this.board[6].toString() + "\n"
    //         + "  " + this.board[0].toString() + " " + this.board[1].toString() + " " + this.board[2].toString() + " "
    //         + this.board[3].toString() + " " + this.board[4].toString() + " " + this.board[5];
    // }

    boardToString() {
        return "--" + this.board[12].toString() + "-" + this.board[11].toString() + "-" + this.board[10].toString() + "-"
            + this.board[9].toString() + "-" + this.board[8].toString() + "-" + this.board[7] + "--\n"
            + '-' + this.board[13].toString() + "------------" + this.board[6].toString() + "-\n"
            + "--" + this.board[0].toString() + "-" + this.board[1].toString() + "-" + this.board[2].toString() + "-"
            + this.board[3].toString() + "-" + this.board[4].toString() + "-" + this.board[5] + "--";
    }

    checkWin() { // return positive if south wins, negative if north, zero if no one
        let south = 0;
        let north = 0;
        for (var i = 0; i < 6; i++) {
            south += this.board[i];
        }
        for (var i = 7; i < 13; i++) {
            north += this.board[i];
        }

        if (south == 0) {
            this.board[13] += north;
            for (var i = 7; i < 13; i++) {
                this.board[i] = 0;
            }
            return this.board[6] - this.board[13];
        } else if (north == 0) {
            this.board[6] += south;
            for (var i = 0; i < 6; i++) {
                this.board[i] = 0;
            }
            return this.board[6] - this.board[13];
        }

        return 0;
    }

    //returns 0 if still playing
    //        1 if ended
    //        -1 if redo turn
    doTurn(initialPit) {
        if (this.turn == 1) {
            initialPit += 7
        }
        if (this.board[initialPit] == 0) {
            console.log("Player chose empty pit, must choose another");
            return -1;
        }

        var pit = initialPit + 1;
        for (var i = 0; i < this.board[initialPit]; i++) {
            //if on last pit roll over to pit zero
            if (pit == 14) {
                pit = 0;
            }
            //if last stone lands in a pit with no stones
            if (i == this.board[initialPit] - 1 && this.board[pit] == 0) {
                if (this.turn == 0 && pit < 6) {
                    this.board[6] += this.board[this.acrossMap[pit]];
                    this.board[this.acrossMap[pit]] = 0;
                } else if (this.turn == 1 && pit > 6 && pit < 13) {
                    this.board[13] += this.board[this.acrossMap[pit]];
                    this.board[this.acrossMap[pit]] = 0;
                }
            }
            this.board[pit] += 1;
            pit += 1;
        }
        this.board[initialPit] = 0;

        var whoWon = this.checkWin();
        if (whoWon != 0) {
            if (whoWon > 0) {
                console.log("SOUTH WINS");
            } else {
                console.log("NORTH WINS");
            }
            return 1;
        }

        if (this.turn == 0) {
            this.turn = 1;
        } else {
            this.turn = 0;
        }
        return 0;
    }
}