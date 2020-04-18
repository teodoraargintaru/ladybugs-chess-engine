//
// Created by Teodora Argintaru on 15/04/2020.
//

#include "eval.h"

// in myPiece vor fi piesele mele de anumit tip
// in opponentPieces piesele adversarului de acelasi tip, doar numarate
// pentru rege nu are sens sa apelam functia, stim ca fiecare player
// are un rege
// pieceAbs == 1 -> regina
void getPiecesOnTableOfType(int pieceAbs, int color, vector<int> &myPiecesOfType,
        int &opponentPieces) {
    int mySign = color == WHITE ? 1 : -1;

    for (auto &entry : positions) {
        int piece = entry.first;
        if (piece == EMPTY) {
            continue;
        }

        if (piece == KING_W || piece == KING_B) {
            continue;
        }

        int pieceSign = piece / abs(piece);

        if (abs(piece) / 10 == pieceAbs) {
            if (pieceSign == mySign) {
                myPiecesOfType.push_back(piece);
            } else {
                opponentPieces++;
            }
        }
    }
}

int calculateScore(vector<int> &myPiecesOfType, int opponentPieces,
        int pieceValue, int values[9][9]) {
    int row, col;
    int score = 0;

    score += pieceValue *  (myPiecesOfType.size() - opponentPieces);

    for(int i = 0; i < myPiecesOfType.size(); i++) {
        row = positions[myPiecesOfType[i]] / 10;
        col = positions[myPiecesOfType[i]] % 10;
        score += values[row][col];
    }

    return score;
}

int evaluate(int color) {
    int opponentColor = color == WHITE ? BLACK : WHITE;
    int score = 0;
    int opponentPieces;
    vector<int> myPiecesOfType;
    int gameStage = 0;
    int myMiddleGamePieces = 0, opponentMiddleGamePieces = 0;

    if (isCheck(color) == true) {
       // printAttacked(color);
        score -= 10000;
    }

    if(isCheck(opponentColor) == true) {
        //printAttacked(opponentColor);
        score += 10000;
    }

    // queens
    opponentPieces = 0;
    getPiecesOnTableOfType(1, color, myPiecesOfType, opponentPieces);
    getPiecesOnTableOfType(6, color, myPiecesOfType, opponentPieces);

    if (opponentPieces == 0 && myPiecesOfType.size() == 0) {
        gameStage = gameStateEnd;
    }

    if (color == WHITE) {
        score += calculateScore(myPiecesOfType, opponentPieces, queenValue,
            whiteQueenSquareTable);
    } else {
        score += calculateScore(myPiecesOfType, opponentPieces, queenValue,
                blackQueenSquareTable);
    }

    // bishop
    myPiecesOfType.clear();
    opponentPieces = 0;
    getPiecesOnTableOfType(2, color, myPiecesOfType, opponentPieces);
    myMiddleGamePieces += myPiecesOfType.size();
    opponentMiddleGamePieces += opponentPieces;
    if (color == WHITE) {
        score += calculateScore(myPiecesOfType, opponentPieces, bishopValue,
                                whiteBishopSquareTable);
    } else {
        score += calculateScore(myPiecesOfType, opponentPieces, bishopValue,
                                blackBishopSquareTable);
    }

    // knight
    myPiecesOfType.clear();
    opponentPieces = 0;
    myMiddleGamePieces += myPiecesOfType.size();
    opponentMiddleGamePieces += opponentPieces;
    getPiecesOnTableOfType(3, color, myPiecesOfType, opponentPieces);
    if (color == WHITE) {
        score += calculateScore(myPiecesOfType, opponentPieces, knightValue,
                                whiteKnightSquareTable);
    } else {
        score += calculateScore(myPiecesOfType, opponentPieces, knightValue,
                                blackKnightSquareTable);
    }

    // game stage
    if (gameStage == 0 && (myMiddleGamePieces > 1 ||
        opponentMiddleGamePieces > 1)) {
        gameStage = gameStateMiddle;
    } else {
        gameStage = gameStateEnd;
    }

    // rook
    myPiecesOfType.clear();
    opponentPieces = 0;
    getPiecesOnTableOfType(4, color, myPiecesOfType, opponentPieces);
    if (color == WHITE) {
        score += calculateScore(myPiecesOfType, opponentPieces, rookValue,
                                whiteRookSquareTable);
    } else {
        score += calculateScore(myPiecesOfType, opponentPieces, rookValue,
                                blackRookSquareTable);
    }

    //pawns
    myPiecesOfType.clear();
    opponentPieces = 0;
    getPiecesOnTableOfType(5, color, myPiecesOfType, opponentPieces);
    if (color == WHITE) {
        score += calculateScore(myPiecesOfType, opponentPieces, pawnValue,
                                whitePawnSquareTable);
    } else {
        score += calculateScore(myPiecesOfType, opponentPieces, pawnValue,
                                blackPawnSquareTable);
    }

    // king
    if (color == WHITE) {
        score += gameStage == gameStateMiddle ?
                 kingValue * whiteKingMiddleGameSquareTable
                                                [positions[KING_W] / 10]
                                                [positions[KING_W] % 10] :
                 kingValue *whiteKingEndGameSquareTable
                                                [positions[KING_W] / 10]
                                                [positions[KING_W] % 10];
    } else {
        score += gameStage == gameStateMiddle ?
                 kingValue * blackKingMiddleGameSquareTable
                                                [positions[KING_B] / 10]
                                                [positions[KING_B] % 10] :
                 kingValue * blackKingEndGameSquareTable
                                                [positions[KING_B] / 10]
                                                [positions[KING_B] % 10];
    }

    return score;
}


// returneaza <scor, piesa * 100 + randDeMutat * 10 + coloanaDeMutat>
pair<int, pair<int, pair<int, int>>> minimax_alphaBeta(int color, int alpha, int beta, int depth) {
    if (depth == 0) {
        //cout<<"Evaluare in frunze "<<evaluate(color)<<endl;
        return make_pair(evaluate(color), make_pair(NULL, make_pair(NULL, NULL)));
    }

    int opponentColor = color == WHITE ? BLACK : WHITE;
    unordered_map<int, vector< pair<int, int> > > moves;

    bool check = isCheck(opponentColor);
    if(check == true) {
        //cout<<"is check opponentcolor"<<opponentColor<<endl;
        if (isMat(opponentColor, moves) == true) {
            return make_pair(INT_MAX, make_pair(NULL, make_pair(NULL, NULL)));
        }
    }

    moves.clear();

    check = isCheck(color);
    if (isCheck(color)) {
        //cout<<"ischeck my color "<<color<<endl;
        if (isMat(color, moves) == true) {
            return make_pair(INT_MIN, make_pair(NULL, make_pair(NULL, NULL)));
        }
    }

    if (moves.size() == 0) {
        //cout<<"Nu e sah"<<endl;
        markAttacked(moves, color);
    }

    int maxScore = INT_MIN;
    pair<int, pair<int, int>> bestMove;


    for (auto &entry : moves) {
        //cout<<entry.first<<" "<<endl;
        int piece = entry.first;
        int rowFrom = positions[piece] / 10;
        int colFrom = positions[piece] % 10;

        for (auto &move : entry.second) {
            int rowTo = move.first;
            int colTo = move.second;

            if (rowTo == rowFrom && colTo == colFrom) {
                continue;
            }
            vector< vector< vector< vector<int> > > > attackedCopy = attacked;
            int captured = applyMoveMinimax(piece, move);

            int score = -1 * (minimax_alphaBeta(opponentColor, -beta, -alpha, depth - 1)).first;

            undoMoveMinimax(piece, captured, rowFrom, colFrom);
            attacked = attackedCopy;

            if (alpha <= score && score <= beta && score >= maxScore) {
                maxScore = score;
                bestMove = make_pair(piece, make_pair(rowTo, colTo));
            }

            alpha = max (alpha, maxScore);

            if(alpha >= beta) {
                break;
            }
        }
    }

    return make_pair(maxScore, bestMove);
}