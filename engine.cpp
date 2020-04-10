//
// Created by Teodora Argintaru on 09/03/2020.
//
#include "engine.h"

void translatePosition(int line, int col, int piece) {
    positions[piece] = line * 10 + col;
}

void initializeBoard() {
    for(int i = 3; i <= 6; i++) {
        for(int j = 1; j <= 8; j++) {
            board[i][j] = EMPTY;
        }
    }

    int pawn_w = PAWN1_W;
    int pawn_b = PAWN1_B;
    for(int i = 1; i <= 8; i++) {
        translatePosition(7, i, pawn_b);
        translatePosition(2, i, pawn_w);
        board[7][i] = pawn_b--;
        board[2][i] = pawn_w++;
    }

    int piece_w = ROOK1_W;
    int piece_b = ROOK1_B;
    for(int i = 1; i <= 3; i++) {
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

void initializeAttacked() {
    for (int i = 0; i < 2; i++) {
        for (int j = 1; j <= 8; j++) {
            for (int k = 1; k <= 8; k++) {
                attacked[i][j][k].clear();
            }
        }
    }
}

void printMap() {
    for(auto& pair : positions) {
        cout<<pair.first<<" "<<pair.second<<endl;
    }
}

void printBoard() {
    for(int i = 8; i >= 1; i--) {
        for(int j = 1; j <= 8; j++) {
            printf("%3d ", board[i][j]);
        }
        cout<<endl;
    }
}

void readFromInput(string &input) {
    getline(cin, input);
    if(input.size() > 77) {
        exit(0);
    }
}

char* makeMove(int colFrom, int colTo, int lineFrom, int lineTo) {
    int piece = board[lineFrom][colFrom];
    if(board[lineFrom][colFrom] != EMPTY) {
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

void translateStringToPositions(char *move, int *colFrom, int *colTo,
        int *lineFrom, int *lineTo) {
    *colFrom = move[0] - 'a' + 1;
    *colTo = move[2] - 'a' + 1;
    *lineFrom = move[1] - '0';
    *lineTo = move[3] - '0';
}

void markMoveOnBoard(char *move) {
    int colFrom, colTo, lineFrom, lineTo;
    translateStringToPositions(move, &colFrom, &colTo, &lineFrom, &lineTo);
    makeMove(colFrom, colTo, lineFrom, lineTo);
}

bool checkCell(int line, int col) {
    return 1 <= line && line <= 8 && 1 <= col && col <= 8;
}

bool checkPiecePosition(int piece) {
    return positions.find(piece) != positions.end();
}

/* pentru CAL si REGE
 * getPieceMoves(KNIGHTXYZ, KNIGHT_MOVES, knightMovesX, knightMovesY)
 * (analog rege)
 *
 * pentru PION
 * va trebui construit pawnMovesY ca un vector<int> pawnMovesY{sign, sign, sign}
 * getPieceMoves(PAWNXYZ, PAWN_MOVES, pawnMovesX, pawnMovesY
 *
 * pentru TURA, NEBUN, REGINA
 * va trebui apelata functia generateMoveVector(piece, movesX, movesY)
 * */
void insertRookMoves(int x, int y, int piece, vector<int> &movesX,
        vector<int> &movesY, vector<int> &myPieceCovered) {
    for(int i = x + 1; i <= 8; i++) {
        if(board[i][y] * piece > 0) {
            myPieceCovered.push_back(board[i][y]);
            break;
        }

        movesX.push_back(i - x);
        movesY.push_back(0);

        if(board[i][y] * piece < 0) {
            break;
        }
    }

    for(int i = 1; i < x; i++){
        if(board[x - i][y] * piece > 0) {
            myPieceCovered.push_back(board[x - i][y]);
            break;
        }

        movesX.push_back(-1 * i);
        movesY.push_back(0);

        if(board[x - i][y] * piece < 0) {
            break;
        }
    }

    for(int i = y + 1; i <= 8; i++) {
        if(board[x][i] * piece > 0) {
            myPieceCovered.push_back(board[x][i]);
            break;
        }

        movesY.push_back(i - y);
        movesX.push_back(0);

        if(board[x][i] * piece < 0) {
            break;
        }
    }

    for(int i = 1; i < y; i++){
        if(board[x][y - i] * piece > 0) {
            myPieceCovered.push_back(board[x][y - i]);
            break;
        }

        movesY.push_back(-1 * i);
        movesX.push_back(0);

        if(board[x][y - i] * piece < 0) {
            break;
        }
    }
}

// vor fi inserate si pozitii redundante (din cauza diferentelor dintre x si y)
// daca avem timp stergem codul duplicat
void insertBishopMoves(int x, int y, int piece, vector<int> &movesX,
        vector<int>&movesY, vector<int> &myPieceCovered) {
    for(int i = 1; i <= 8 && x + i <= 8 && y + i <= 8; i++) {
        if(board[x + i][y + i] * piece > 0) {
            myPieceCovered.push_back(board[x + i][y + i]);
            break;
        }

        movesX.push_back(i); // +
        movesY.push_back(i); // +

        if(board[x + i][y + i] * piece < 0) {
            break;
        }
    }

    // creste pe x si scade pe y
    for(int i = 1; i <= 8 && x + i <= 8 && y - i >= 1; i++) {
        if(board[x + i][y - i] * piece > 0) {
            myPieceCovered.push_back(board[x + i][y - i]);
            break;
        }

        movesX.push_back(i);
        movesY.push_back(-1 * i);

        if(board[x + i][y - i] * piece < 0) {
            break;
        }
    }

    for(int i = 1; i <= 8 && x - i >= 1 && y - i >= 1; i++) {
        if(board[x - i][y - i] * piece > 0) {
            myPieceCovered.push_back(board[x - i][y - i]);
            break;
        }

        movesX.push_back(-1 * i);
        movesY.push_back(-1 * i);

        if(board[x - i][y - i] * piece < 0) {
            break;
        }
    }

    for(int i = 1; i <= 8 && x - i >= 1 && y + i <= 8; i++) {
        if(board[x - i][y + i] * piece > 0) {
            myPieceCovered.push_back(board[x - i][y + i]);
            break;
        }

        movesX.push_back(-1 * i); // +
        movesY.push_back(i); // +

        if(board[x - i][y + i] * piece < 0) {
            break;
        }
    }
}

void generateMoveVectors(int piece, vector<int> &movesX, vector<int> &movesY,
        vector<int> &myPieceCovered) {
    if(checkPiecePosition(piece) == false) {
        return;
    }

    int x = positions[piece] / 10;
    int y = positions[piece] % 10;

    if(piece == ROOK1_B || piece == ROOK1_W ||
        piece == ROOK2_B || piece == ROOK2_W) {
        insertRookMoves(x, y, piece, movesX, movesY, myPieceCovered);
        return;
    }

    if(piece == BISHOP1_B || piece == BISHOP1_W ||
        piece == BISHOP2_B || piece == BISHOP2_W) {
        insertBishopMoves(x, y, piece, movesX, movesY, myPieceCovered);
        return;
    }

    if(piece == QUEEN_W || piece == QUEEN_B) {
        insertRookMoves(x, y, piece, movesX, movesY, myPieceCovered);
        insertBishopMoves(x, y, piece, movesX, movesY, myPieceCovered);
    }
}

vector<string> getPieceMoves(int piece, int movesNr,
        vector<int> &movesX, vector<int> &movesY,
        vector<int> &newMovesX, vector<int> &newMovesY,
        vector<int> &myPieceCovered) {
    vector<string> possibleMoves;

    if(checkPiecePosition(piece) == false) {
        return possibleMoves;
    }

    string move = "";
    int type = abs(piece) / 10;

    for(int i = 0; i < movesNr; i++) {
        int toRow = positions[piece] / 10 + movesX[i];
        int toCol = positions[piece] % 10 + movesY[i];
        if(checkCell(toRow, toCol) == true &&
                (board[toRow][toCol] * piece <= 0)) {
                move = string(makeMove(positions[piece] % 10,
                                       positions[piece] % 10 + movesY[i],
                                       positions[piece] / 10,
                                       positions[piece] / 10 + movesX[i]));
                newMovesX.push_back(toRow);
                newMovesY.push_back(toCol);
                //(x, y)
                possibleMoves.push_back(move);
        } else if (checkCell(toRow, toCol) == true &&
            (board[toRow][toCol] * piece > 0)) {
            myPieceCovered.push_back(board[toRow][toCol]);
        }
    }

    return possibleMoves;
}

void markAttacked() {
    vector<string> moves;
    for (auto &entry : positions) {
        vector<int> newMovesX, newMovesY, myPieceCovered;

        int piece = entry.first;
        int sign = piece < 0 ? -1 : 1;
        int color = piece < 0 ? BLACK : WHITE;

        if (abs(piece) / 10 == 5) { // pion
            vector<int> pawnMovesY(3, sign);
            getPieceMoves(piece, PAWN_MOVES, pawnMovesX,
                    pawnMovesY, newMovesX, newMovesY, myPieceCovered);
        } else if (abs(piece) / 10 == 3) { // cal
            getPieceMoves(piece, KNIGHT_MOVES, knightMovesX, knightMovesY,
                    newMovesX, newMovesY, myPieceCovered);
        } else if (piece == KING_W || piece == KING_B) { // rege
            getPieceMoves(piece, KING_MOVES, kingMovesX, kingMovesY,
                    newMovesX, newMovesY, myPieceCovered);
        } else {
            vector<int> movesX, movesY;
            generateMoveVectors(piece, movesX, movesY, myPieceCovered);
            getPieceMoves(piece, movesX.size(), movesX, movesY, newMovesX,
                    newMovesY, myPieceCovered);
        }

        for (int i = 0; i < newMovesX.size(); i++) {
            attacked[color - 1][newMovesX[i]][newMovesY[i]].push_back(piece);
        }

        for(auto &piece : myPieceCovered) {
            int position = positions[piece];
            attacked[color - 1][position / 10][position % 10].push_back(piece);
        }
    }
}

bool kingMobility(int king, int color) {
    int row = positions[king] / 10;
    int col = positions[king] % 10;
    bool ok = false;
    int toRow, toCol;
    int opponentColor = color == WHITE ? BLACK : WHITE;

    for(int i = 0; i < KING_MOVES; i++) {
        toRow = row + kingMovesX[i];
        toCol = col + kingMovesY[i];
        if(checkCell(toRow, toCol) == true && // e pe tabla
            board[toRow][toCol] * king <= 0 && // piesa diferita
            attacked[opponentColor - 1][toRow][toCol].size() == 0) {
            // pozitia nu e atacata
            ok = true;
        }
    }

    return ok;
}

bool isMat(int color) {
    int king = color == WHITE ? KING_W : KING_B;
    int position = positions[king];

    // pot sa ma mut
    if(kingMobility(king, color == true)) {
        return false;
    }

    // nu pot sa ma mut si sunt atacat din mai mult de 2 parti
    if(attacked[color - 1][position / 10][position % 10].size() >= 2) {
        return true;
    }

    // sah dintr-o singura parte, pot sa iau piesa care ma ataca
    int attacker = attacked[color - 1][position / 10][position % 10][0];
    int attackerPosition = positions[attacker];
    if(attacked[color - 1]
        [attackerPosition / 10][attackerPosition % 10].size() > 0) {
        return false;
    }

    // pot sa blochez (doar tura, regina, nebun)
    // nu poate sa fie rege sau pion, calului nu am ce sa ii fac
    int minX = min(position / 10, attackerPosition / 10);
    int maxX = max(position / 10, attackerPosition / 10);
    int minY = min(position % 10, attackerPosition % 10);
    int maxY = max(position % 10, attackerPosition % 10);

    if (abs(attacker) / 10 == 4 || abs(attacker) / 10 == 1) {
        // tura sau regina
        for (int i = minX + 1; i < maxX; i++) {
            if(attacked[color - 1][i][minY].size() > 0) {
                return false;
            }
        }

        for (int i = minY + 1; i < maxY; i++) {
            if(attacked[color - 1][minX][i].size() > 0) {
                return false;
            }
        }
    }

    if (abs(attacker) / 10 == 2 || abs(attacker) / 10 == 1) {
        // nebun sau regina
        int directionX = position / 10 > attackerPosition / 10 ? -1 : 1;
        int directionY = position % 10 > attackerPosition % 10 ? -1 : 1;
        for (int i = position / 10 + directionX,
                j = position % 10 + directionY;
                i != attackerPosition / 10;
                i += directionX, j += directionY) {
            if(attacked[color - 1][i][j].size() > 0) {
                return false;
            }
        }
    }

    return true;
}

bool isCheck(int color) {
    int king = color == WHITE ? KING_W : KING_B;
    int position = positions[king];
    return (attacked[color - 1][position / 10][position % 10].size() > 0);
}

bool movePawn(int pawn) {
    int sign = pawn < 0 ? -1 : 1;

    if(checkPiecePosition(pawn) == false) {
        return false;
    }

    if(checkCell(positions[pawn] / 10 + sign, positions[pawn] % 10 + 1) == true
       &&
       board[positions[pawn] / 10 + sign][positions[pawn] % 10 + 1] != EMPTY) {
        cout<<"move "<<makeMove(positions[pawn] % 10,
                positions[pawn] % 10 + 1,positions[pawn] / 10,
                positions[pawn] / 10 + sign)<<endl;
        return true;
    }
    if(checkCell(positions[pawn] / 10 + sign, positions[pawn] % 10 - 1) == true
       &&
       board[positions[pawn] / 10 + sign][positions[pawn] % 10 - 1] != EMPTY) {
        cout<<"move "<<makeMove(positions[pawn] % 10,
                positions[pawn] % 10 - 1,positions[pawn] / 10,
                positions[pawn] / 10 + sign)<<endl;
        return true;
    }
    if(checkCell(positions[pawn] / 10 + sign, positions[pawn] % 10) == true &&
       board[positions[pawn] / 10 + sign][positions[pawn] % 10] == EMPTY) {
        cout<<"move "<<makeMove(positions[pawn] % 10, positions[pawn] % 10,
                                positions[pawn] / 10,
                                positions[pawn] / 10 + sign)<<endl;
        return true;
    }

    return false;
}

void applyStrategy() {
    bool legal = true;
    if(engineColor == WHITE) {
        legal = movePawn(PAWN1_W);
    } else if(engineColor == BLACK) {
        legal = movePawn(PAWN1_B);
    }
    if(legal == false){
        cout<<"resign\n";
    }
}
