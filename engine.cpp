//
// Created by Teodora Argintaru on 09/03/2020.
//
#include "engine.h"


void translatePosition(int line, int col, int piece) {
    positions[piece] = line * 10 + col;
}

void initializeBoard() {
    for(int i = 3; i <= 6; i++){
        for(int j = 1; j <= 8; j++){
            board[i][j] = EMPTY;
        }
    }

    int pawn_w = PAWN1_W;
    int pawn_b = PAWN1_B;
    for(int i = 1; i <= 8; i++){
        translatePosition(7, i, pawn_b);
        translatePosition(2, i, pawn_w);
        board[7][i] = pawn_b--;
        board[2][i] = pawn_w++;
    }

    int piece_w = ROOK1_W;
    int piece_b = ROOK1_B;

    for(int i = 1; i <= 3; i++){
        translatePosition(1, i, piece_w);
        translatePosition(1, 8 - i + 1, piece_w + 1);
        board[1][i] = piece_w++;
        board[1][8 - i + 1] = piece_w;
        piece_w -= 11;

        translatePosition(8, i, piece_b);
        translatePosition(8, 8 - i + 1, piece_b - 1);
        board[8][i] = piece_b--;
        board[8][8 - i + 1] = piece_b;
        piece_b += 11;
    }

    board[1][4] = QUEEN_W;
    board[1][5] = KING_W;
    board[8][4] = QUEEN_B;
    board[8][5] = KING_B;
    translatePosition(1, 4, QUEEN_W);
    translatePosition(1, 5, KING_W);
    translatePosition(8, 4, QUEEN_B);
    translatePosition(8, 5, KING_B);
}

void printMap() {
    for(auto& pair : positions){
        cout<<pair.first<<" "<<pair.second<<endl;
    }
}

void printBoard() {
    for(int i = 8; i >= 1; i--){
        for(int j = 1; j <= 8; j++){
            printf("%3d ", board[i][j]);
        }
        cout<<endl;
    }
}

void readFromInput(string &input) {
    getline(cin, input);
    if(input.size() > 77){
        exit(0);
    }
}

char * makeMove(int colFrom, int colTo, int lineFrom, int lineTo) {
    int piece = board[lineFrom][colFrom];
    if(board[lineFrom][colFrom] != EMPTY){
        positions.erase(board[lineFrom][colFrom]);
    }
    board[lineFrom][colFrom] = EMPTY;
    board[lineTo][colTo] = piece;
    translatePosition(lineTo, colTo, piece);

    char* move = (char *)malloc(5);
    move[0] = colFrom + 'a' - 1;
    move[1] = lineFrom + '0';
    move[2] = colTo + 'a' - 1;
    move[3] = lineTo + '0';
    move[4] = '\0';
    strcpy(lastMove, move);

    return move;
}

void markMoveOnBoard(char *move) {
    int colFrom = move[0] - 'a' + 1;
    int colTo = move[2] - 'a' + 1;
    int lineFrom = move[1] - '0';
    int lineTo = move[3] - '0';

    makeMove(colFrom, colTo, lineFrom, lineTo);
}

bool checkCell(int line, int col) {
    return 1 <= line && line <= 8 && 1 <= col && col <= 8;
}

bool checkPiecePosition(int piece) {
    return positions.find(piece) != positions.end();
}

bool movePawn(int pawn) {
    int sign = pawn < 0 ? -1 : 1;

    if(checkPiecePosition(pawn) == false){
        return false;
    }

    if(checkCell(positions[pawn] / 10 + sign, positions[pawn] % 10 + 1) == true &&
       board[positions[pawn] / 10 + sign][positions[pawn] % 10 + 1] != EMPTY){
        cout<<"move "<<makeMove(positions[pawn] % 10, positions[pawn] % 10 + 1, positions[pawn] / 10,
                                positions[pawn] / 10 + sign)<<endl;
        return true;
    }
    if(checkCell(positions[pawn] / 10 + sign, positions[pawn] % 10 - 1) == true &&
       board[positions[pawn] / 10 + sign][positions[pawn] % 10 - 1] != EMPTY){
        cout<<"move "<<makeMove(positions[pawn] % 10, positions[pawn] % 10 - 1, positions[pawn] / 10,
                                positions[pawn] / 10 + sign)<<endl;
        return true;
    }
    if(checkCell(positions[pawn] / 10 + sign, positions[pawn] % 10) == true &&
       board[positions[pawn] / 10 + sign][positions[pawn] % 10] == EMPTY){
        cout<<"move "<<makeMove(positions[pawn] % 10, positions[pawn] % 10, positions[pawn] / 10,
                                positions[pawn] / 10 + sign)<<endl;
        return true;
    }

    return false;
}

void applyStrategy() {
    bool legal = true;
    if(engineColor == WHITE){
        legal = movePawn(PAWN1_W);
    } else if(engineColor == BLACK){

        legal = movePawn(PAWN1_B);
    }
    if(legal == false){
        cout<<"resign\n";
    }
}