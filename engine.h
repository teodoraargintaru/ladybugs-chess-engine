//
// Created by Teodora Argintaru on 09/03/2020.
//

#ifndef LADYBUGS_ENGINE_H
#define LADYBUGS_ENGINE_H

#include "codifications.h"
inline int whiteKingMoved = 0;
inline int blackKingMoved = 0;
inline int whiteRookLeftMoved = 0;
inline int whiteRookRightMoved = 0;
inline int blackRookLeftMoved = 0;
inline int blackRookRightMoved = 0;

inline int promotedWhite = 0;
inline int promotedBlack = 0;
inline pair<int, int> promotedPawn;
inline bool promotionIsSet = false;

inline int pawn2moves = EMPTY;
inline bool enPassant = false;
inline int board[9][9] = {0};
// positions[piece] = line * 10 + col;
inline unordered_map<int, int> positions;
inline bool force = false;
inline char lastMove[5];
inline int colorToMove = WHITE, engineColor = NONE;

inline vector<int> knightMovesX {2, 1, -1, -2, -2, -1, 1, 2};
inline vector<int> knightMovesY {1, 2, 2, 1, -1, -2, -2, -1};
inline int KNIGHT_MOVES = 8;

inline vector<int> pawnMovesX {1, -1, 0};
inline int PAWN_MOVES = 3;

inline vector<int> kingMovesX {-1, 0, 1, -1, 1, -1, 0, 1};
inline vector<int> kingMovesY {-1, -1, -1, 0, 0, 1, 1, 1};
inline int KING_MOVES = 8;

inline vector< vector< vector< vector<int> > > > attacked
    (2, vector< vector< vector< int> > >
        (9, vector< vector < int> >
                (9, vector <int> (0, 0))));

// calls insertRookMoves/insertBishopMoves or both
void generateMoveVectors(int piece, vector<int> &movesX, vector<int> &movesY);

// returns a vector of strings -> possible moves that can be written to stdout
// for each piece
vector<string> getPieceMoves(int piece, int movesNr,
                             vector<int> &movesX, vector<int> &movesY);

// functions that generate moves vectors for bishop/rooks
void insertRookMoves(int x, vector<int> &movesX, vector<int> &movesY);
void insertBishopMoves(int x, vector<int> &movesX, vector<int>&movesY);

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

void markAttacked(unordered_map<int, vector< pair<int, int> > > &moves,
                  int coloToMove);

bool isCheck(int color);

bool isMat(int color, unordered_map<int, vector< pair<int, int> > > &moves);

int applyMoveMinimax(int piece, pair<int, int> move);

void undoMoveMinimax(int piece, int captured, int initialRow, int initialCol);

void printAttacked(int color);
#endif //LADYBUGS_CHESS_ENGINE_ENGINE_H