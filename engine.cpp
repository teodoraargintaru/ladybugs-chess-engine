//
// Created by Teodora Argintaru on 09/03/2020.
//
#include "engine.h"
#include "eval.h"

void translatePosition(int line, int col, int piece) {
    positions[piece] = line * 10 + col;
}

void initializeBoard() {
    positions.clear();
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
    char* move = (char *)malloc(5);
    move[0] = colFrom + 'a' - 1;
    move[1] = lineFrom + '0';
    move[2] = colTo + 'a' - 1;
    move[3] = lineTo + '0';
    move[4] = '\0';
    strcpy(lastMove, move);

    return move;
}

//void makeMoveForMinimax()
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
    //makeMove(colFrom, colTo, lineFrom, lineTo);
    applyMoveMinimax(board[lineFrom][colFrom], make_pair(lineTo, colTo));
}

bool checkCell(int line, int col) {
    return 1 <= line && line <= 8 && 1 <= col && col <= 8;
}

bool checkPiecePosition(int piece) {
    return positions.find(piece) != positions.end();
}

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

    // regine vechi sau regine promovate
    if(piece == QUEEN_W || piece == QUEEN_B || abs(piece) / 10 == 6) {
        insertRookMoves(x, y, piece, movesX, movesY, myPieceCovered);
        insertBishopMoves(x, y, piece, movesX, movesY, myPieceCovered);
    }
}

void getPieceMoves(int piece, int movesNr,
        vector<int> &movesX, vector<int> &movesY,
        vector<int> &newMovesX, vector<int> &newMovesY,
        vector<int> &myPieceCovered) {
    if(checkPiecePosition(piece) == false) {
        return;
    }

    int type = abs(piece) / 10;

    for(int i = 0; i < movesNr; i++) {
        int toRow = positions[piece] / 10 + movesX[i];
        int toCol = positions[piece] % 10 + movesY[i];
        if(checkCell(toRow, toCol) == true &&
                (board[toRow][toCol] * piece <= 0)) {
                newMovesX.push_back(toRow);
                newMovesY.push_back(toCol);
        } else if (checkCell(toRow, toCol) == true &&
            (board[toRow][toCol] * piece > 0)) {
            myPieceCovered.push_back(board[toRow][toCol]);
        }
    }
}

void getPieceMovesPawn(int piece, int movesNr,
        vector<int> &newMovesX, vector<int> &newMovesY,
        vector<int> &myPieceCovered,
        vector<int> &attackButNoMoveX, vector<int> &attackButNoMoveY) {
    if(checkPiecePosition(piece) == false) {
        return;
    }

    int sign = piece < 0 ? -1 : 1;
    int firstLine = piece < 0 ? 7 : 2;
    int position = positions[piece];
    int row = position / 10;
    int col = position % 10;

    if (row == firstLine && board[row + sign * 2][col] == 0) {
        newMovesX.push_back(row + sign * 2);
        newMovesY.push_back(col);
    }

    if (checkCell(row + sign, col) == true && board[row + sign][col] == 0) {
        newMovesX.push_back(row + sign);
        newMovesY.push_back(col);
    }

    vector<int> diag {1, -1};
    for(auto i : diag){
        if (checkCell(row + sign, col + i) == false) {
            continue;
        }
        if (board[row + sign][col + i] * piece < 0) {
            newMovesX.push_back(row + sign);
            newMovesY.push_back(col + i);
        } else if (board[row + sign][col + i] * piece > 0) {
            myPieceCovered.push_back(board[row + sign][col + i]);
        } else {
            attackButNoMoveX.push_back(row + sign);
            attackButNoMoveY.push_back(col + i);
        }
    }

    if (pawn2moves != EMPTY) {
        // e pe aceeasi rand cu mine, pe coloana de langa
        if (abs(col - positions[pawn2moves] % 10) == 1) {
            newMovesX.push_back(row + sign);
            newMovesY.push_back(positions[pawn2moves] % 10);
        }
    }
}

//intoarce piesa capturata
int applyMoveMinimax(int piece, pair<int, int> move) {
    int lineFrom = positions[piece] / 10;
    int colFrom = positions[piece] % 10;
    int lineTo = move.first;
    int colTo = move.second;
    int captured = EMPTY;
    int sign = piece < 0 ? -1 : 1;
    int color = sign < 0 ? BLACK : WHITE;
    enPassant = false;

    if (pawn2moves != EMPTY) {
        // e pe aceeasi rand cu mine, pe coloana de langa
        // si vreau sa fac mutarea de enpassant
        if (abs(piece / 10) == 5 && abs(colFrom - positions[pawn2moves] % 10) == 1
            && colTo == positions[pawn2moves] % 10) {
            captured = pawn2moves;
            positions.erase(pawn2moves);
            enPassant = true;
        }
    }

    // daca e pion si mut 2 in fata
    if (abs(piece) / 10 == 5 && abs(lineFrom - lineTo) == 2) {
        pawn2moves = piece;
    } else {
        pawn2moves = EMPTY;
    }

    if (board[lineTo][colTo] != EMPTY) {
        captured = board[lineTo][colTo];
        positions.erase(board[lineTo][colTo]);
    }

    board[lineFrom][colFrom] = EMPTY;

    // pawn promotion
    if (abs(piece) / 10 == 5 && (lineTo == 8 || lineTo == 1)) {
        positions.erase(piece);
        promotionIsSet = true;
        int oldPiece = piece;

        if(sign == 1) {
            piece = PROMOTED_QUEEN_W + promotedWhite;
            promotedWhite++;
        } else {
            piece = PROMOTED_QUEEN_B - promotedBlack;
            promotedBlack++;
        }

        promotedPawn = {oldPiece, piece};
    } else {
        promotionIsSet = false;
    }

    // castling -> mutarea turei
    // regele oricum se muta
    if ((piece == KING_B || piece == KING_W) && abs(colFrom - colTo) == 2) {
        // rocada mica -> negativ
        // rocada mare -> pozitiv
        int castlingType = (colFrom - colTo) / 2;

        // incepem de la tura din dreapta
        int rook = sign * 42;

        // daca e rocada mare
        if (castlingType > 0) {
            rook -= sign;
        }

        int rookCol = positions[rook] % 10;
        int rookRow = lineTo;

        board[rookRow][rookCol] = EMPTY;
        //rocada mare -> colrege + 1
        //rocada mica -> colrege - 1
        board[rookRow][colTo + castlingType] = rook;
        translatePosition(rookRow, colTo + castlingType, rook);
    }

    board[lineTo][colTo] = piece;
    translatePosition(lineTo, colTo, piece);

    unordered_map<int, vector< pair<int, int> > > moves; //dummy
    markAttacked(moves, color);

    moves.clear();
    int opponentColor = color == WHITE ? BLACK : WHITE;
    markAttacked(moves, opponentColor);

    return captured;
}

// sa avem grija sa pastram o copie a tabelei de atacate
// si sa o reinitializam cand iesim din minimax
// si sa salvam si unde era inainte
void undoMoveMinimax(int piece, int captured, int initialRow, int initialCol) {
    // unde se afla piesa mea acum
    int lineFrom = positions[piece] / 10;
    int colFrom = positions[piece] % 10;

    if (captured != EMPTY) {
        if (enPassant == true) {
            board[initialRow][colFrom] = captured;
            translatePosition(initialRow, colFrom, captured);
            enPassant = false;
            pawn2moves = captured;
        } else {
            translatePosition(lineFrom, colFrom, captured);
        }
    }

    if (enPassant == false) {
        board[lineFrom][colFrom] = captured;
    }

    if (promotionIsSet == true) {
        positions.erase(promotedPawn.second); //sterg regina
        piece = promotedPawn.first;
        promotionIsSet = false;
    }

    board[initialRow][initialCol] = piece;
    translatePosition(initialRow, initialCol, piece);
}

bool checkLeftRightCastling(int color, int rowKing, bool rookMoved, int endCol,
        int offset, int rook) {
    int attackerColor = color == WHITE ? BLACK : WHITE;
    if (rookMoved == false) {
        // daca regele este atacat de tura mea, atunci nu se afla nimic intre ele
        bool isAttackedBetween = false;

        if (find(attacked[color - 1][rowKing][5].begin(),
                 attacked[color - 1][rowKing][5].end(), rook) !=
            attacked[color - 1][rowKing][5].end()) {
            for (int i = 5; i != endCol; i += offset) {
                if (attacked[attackerColor - 1][rowKing][i].size() > 0) {
                    isAttackedBetween = true;
                    return false;
                }
            }

            if (isAttackedBetween == false) {
                return true;
            }
        }
    }

    return false;
}

void canDoCastling(int color,
        unordered_map<int, vector< pair<int, int> > > &moves) {
    if (isCheck(color) == true) {
        return;
    }

    bool kingMoved = color == WHITE ?
            whiteKingMoved : blackKingMoved;
    bool rookLeftMoved = color == WHITE ?
            whiteRookLeftMoved : blackRookLeftMoved;
    bool rookRightMoved = color == WHITE ?
            whiteRookRightMoved : blackRookRightMoved;
    int rowKing = color == WHITE ? 1 : 8;
    int rookRight = color == WHITE ? ROOK2_W : ROOK2_B;
    int rookLeft = color == WHITE ? ROOK1_W : ROOK1_B;
    int king = color == WHITE ? KING_W : KING_B;

    if (kingMoved == true) {
        return;
    }

    //rocada mica
    if (checkLeftRightCastling(color, rowKing, rookRightMoved,
            7, 1, rookRight) == true) {
        moves[king].push_back(make_pair(rowKing, 7));
    }

    //rocada mare
    if (checkLeftRightCastling(color, rowKing, rookLeftMoved,
            3, -1, rookLeft) == true) {
        moves[king].push_back(make_pair(rowKing, 3));
    }
}

// completeaza tabela atacat pentru ambele culori
// genereaza viitoarele miscari in moves doar pentru culoarea care e la mutare
void markAttacked(unordered_map<int, vector< pair<int, int> > > &moves,
        int myColorToMove) {
    initializeAttacked();
    vector< pair<int, int> > dummy;

    for (auto &entry : positions) {
        vector<int> newMovesX, newMovesY, myPieceCovered;
        vector<int> attackButNoMoveX, attackButNoMoveY;
        int piece = entry.first;
        int sign = piece < 0 ? -1 : 1;
        int color = piece < 0 ? BLACK : WHITE;

        if (abs(piece) / 10 == 5) { // pion
            getPieceMovesPawn(piece, PAWN_MOVES, newMovesX, newMovesY,
                    myPieceCovered, attackButNoMoveX, attackButNoMoveY);
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

        if (color == myColorToMove) {
            moves[piece] = dummy;
            for (int i = 0; i < newMovesX.size(); i++) {
                moves[piece].push_back(make_pair(newMovesX[i], newMovesY[i]));
            }
        }

        for (int i = 0; i < newMovesX.size(); i++) {
            // daca e pion si pozitia e pe aceeasi coloana
            if (abs(piece) / 10 == 5 && newMovesY[i] == positions[piece] % 10) {
                continue;
            }
            attacked[color - 1][newMovesX[i]][newMovesY[i]].push_back(piece);
        }

        for (int i = 0; i < attackButNoMoveX.size(); i++) {
            attacked[color - 1][attackButNoMoveX[i]][attackButNoMoveY[i]].push_back(piece);
        }

        for(auto &piece : myPieceCovered) {
            int position = positions[piece];
            attacked[color - 1][position / 10][position % 10].push_back(piece);
        }
    }

    canDoCastling(colorToMove, moves);
}

void kingMobility(int king, int color,
        unordered_map<int, vector< pair<int, int> > > &moves) {
    int row = positions[king] / 10;
    int col = positions[king] % 10;
    bool ok = false;
    int toRow, toCol;
    int opponentColor = color == WHITE ? BLACK : WHITE;

    vector<pair<int, int>> dummy;
    if (moves.find(king) == moves.end()) {
        moves[king] = dummy;
    }

    for(int i = 0; i < KING_MOVES; i++) {
        toRow = row + kingMovesX[i];
        toCol = col + kingMovesY[i];
        if(checkCell(toRow, toCol) == true && // e pe tabla
            board[toRow][toCol] * king <= 0 && // piesa diferita
            attacked[opponentColor - 1][toRow][toCol].size() == 0) {
            // pozitia nu e atacata
            moves[king].push_back(make_pair(toRow, toCol));
            ok = true;
        }
    }
}

void insertInMovesMap(unordered_map<int, vector< pair<int, int> > > &moves,
        int row, int col, int color) {
    vector< pair<int, int> > dummy;

    for (auto conterAttack : attacked[color - 1][row][col]) {
        if (moves.find(conterAttack) == moves.end()) {
            moves[conterAttack] = dummy;
        }

        moves[conterAttack].push_back(make_pair(row, col));
    }
}

bool isMat(int color, unordered_map<int, vector< pair<int, int> > > &moves) {
    int king = color == WHITE ? KING_W : KING_B;
    int position = positions[king];
    vector< pair<int, int> > dummy;
    // pot sa ma mut
    kingMobility(king, color, moves);

    // nu pot sa ma mut si sunt atacat din mai mult de 2 parti
    if(moves.size() == 0 &&
        attacked[color - 1][position / 10][position % 10].size() >= 2) {
        return true;
    }

    // sah dintr-o singura parte, pot sa iau piesa care ma ataca
    int attacker = attacked[color - 1][position / 10][position % 10][0];
    int attackerPosition = positions[attacker];

    if (attacked[color - 1]
        [attackerPosition / 10][attackerPosition % 10].size() > 0) {
        insertInMovesMap(moves, position / 10, position % 10, color);
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
                insertInMovesMap(moves, i, minY, color);
            }
        }

        for (int i = minY + 1; i < maxY; i++) {
            if(attacked[color - 1][minX][i].size() > 0) {
                insertInMovesMap(moves, minX, i, color);
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
                insertInMovesMap(moves, i, j, color);
            }
        }
    }

    return moves.size() > 0 ? false : true;
}

// returneaza true daca eu sunt in sah
bool isCheck(int color) {
    int king = color == WHITE ? KING_W : KING_B;
    int position = positions[king];
    int opponentColor = color == WHITE ? BLACK : WHITE;
    return (attacked[opponentColor - 1][position / 10][position % 10].size() > 0);
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
    /*bool legal = true;
    if(engineColor == WHITE) {
        legal = movePawn(PAWN1_W);
    } else if(engineColor == BLACK) {
        legal = movePawn(PAWN1_B);
    }
    if(legal == false){
        cout<<"resign\n";
    }*/


    pair<int, pair<int, pair<int, int>>> minimaxResult;
    cout<<"Engine color is "<<engineColor<<endl;
    minimaxResult = minimax_alphaBeta(engineColor, -1234567, 1234567, 2);
    //minimaxResult = make_pair(INT_MAX, make_pair(PAWN3_B, make_pair(6, 3)));
    int piece = minimaxResult.second.first;
    int rowTo = minimaxResult.second.second.first;
    int colTo = minimaxResult.second.second.second;
    int rowFrom = positions[piece] / 10;
    int colFrom = positions[piece] % 10;

    applyMoveMinimax(piece, make_pair(rowTo, colTo));

    if(promotionIsSet == true) {
        cout<<"move "<<makeMove(colFrom, colTo, rowFrom, rowTo)<<"q"<<endl;
        return;
    }

    if(piece == KING_B || (piece == KING_W && abs(colTo - colFrom) > 1)) {
        int direction = colTo - colFrom; // 0 -> dreapta
        if (engineColor == WHITE) {
            if(direction > 0) {
                cout << "move e1g1" << endl;
            } else {
                cout << "move e1c1" << endl;
            }
            return;
        } else {
            if(direction > 0) {
                cout << "move e8g8" << endl;
            } else {
                cout << "move e8c8" << endl;
            }
            return;
        }

    }

    cout<<minimaxResult.first<<endl;
    cout<<"move "<<makeMove(colFrom, colTo, rowFrom, rowTo)<<endl;
}