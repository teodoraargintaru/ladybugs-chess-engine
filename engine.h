//
// Created by Teodora Argintaru on 09/03/2020.
//

#ifndef LADYBUGS_CHESS_ENGINE_ENGINE_H
#define LADYBUGS_CHESS_ENGINE_ENGINE_H

#include "codifications.h"

inline int board[9][9] = {0};
// positions[piece] = line * 10 + col;
inline unordered_map<int, int> positions;
inline bool force = false;
inline char lastMove[5];
inline int colorToMove = WHITE, engineColor = NONE;

// translates position from board[line][col] to positions map value
void translatePosition(int line, int col, int piece);

// sets pieces back to starting positions
void initializeBoard();

// helper methods
void printMap();
void printBoard();

// reads from stdin and checks if size is < 77
void readFromInput(string &input);

// makes move on board
// returns string to print for move command
char* makeMove(int colFrom, int colTo, int lineFrom, int lineTo);

// translates string to board positions and then calls makeMove
void markMoveOnBoard(char *move);

// checks if cell is on board
bool checkCell(int line, int col);

// checks if piece is still on board (hasn't been taken yet)
bool checkPiecePosition(int piece);

// dummy method to move pawn until there are no valid moves left
bool movePawn(int pawn);

// tries to make move. if there are no valid moves left it resigns
void applyStrategy();

#endif //LADYBUGS_CHESS_ENGINE_ENGINE_H
