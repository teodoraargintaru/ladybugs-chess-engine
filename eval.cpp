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
        score -= 200000;
    }

    if(isCheck(opponentColor) == true) {
        //printAttacked(opponentColor);
        score += 200000;
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
        return make_pair(evaluate(color), make_pair(EMPTY, make_pair(NULL, NULL)));
    }

    int opponentColor = color == WHITE ? BLACK : WHITE;
    unordered_map<int, vector< pair<int, int> > > moves;

    bool check = isCheck(opponentColor);
    if(check == true) {
        //cout<<"is check opponentcolor"<<opponentColor<<endl;
        if (isMat(opponentColor, moves) == true) {
            return make_pair(999997, make_pair(EMPTY, make_pair(NULL, NULL)));
        }
    }

    moves.clear();

    check = isCheck(color);
    if (isCheck(color)) {
        //cout<<"ischeck my color "<<color<<endl;
        if (isMat(color, moves) == true) {
            return make_pair(-999997, make_pair(EMPTY, make_pair(NULL, NULL)));
        }
        cout<< " moves size; " << moves.size()<<endl;
    }

    if (moves.size() == 0) {
        //cout<<"Nu e sah"<<endl;
        markAttacked(moves, color);
    }

    /*
    if (moves.size() == 1) {
        int king = color == WHITE ? KING_W : KING_B;
        vector<pair<int, int>> availableMove = moves[king];
        return make_pair(200000, make_pair(king, availableMove[0]));
    }*/

    int maxScore = -999999;
    pair<int, pair<int, int>> bestMove;
    bool existMoves = false;

    for (auto &entry : moves) {
        //cout<<entry.first<<" "<<endl;
        int piece = entry.first;
        if (piece == 0) {
            continue;
        }

        int rowFrom = positions[piece] / 10;
        int colFrom = positions[piece] % 10;
        printf("Adancime %d piece %d row %d col %d\n", depth, piece, rowFrom, colFrom);
        for (auto &move : entry.second) {
            int rowTo = move.first;
            int colTo = move.second;

            if (rowTo == rowFrom && colTo == colFrom) {
                continue;
            }

            bool promotionIsSet = false;
            vector< vector< vector< vector<int> > > > attackedCopy = attacked;
            int captured = applyMoveMinimax(piece, move, promotionIsSet);

            if (isCheck(color) == true) {
                undoMoveMinimax(piece, captured, rowFrom, colFrom, promotionIsSet);
                continue;
            }

            int score = -1 * minimax_alphaBeta(opponentColor, -beta, -alpha, depth - 1).first;

            undoMoveMinimax(piece, captured, rowFrom, colFrom, promotionIsSet);
            attacked = attackedCopy;

            /*
             * if (score > bestScore)
//                {
//                    bestScore = score;
//                    bestMove = moves[i].HashKey;
//                }
//                if (bestScore > alpha)
//                {
//                    alpha = bestScore;
//                }
//                if (bestScore >= beta)
//                {
//                    TTCutoffs++;
//                    // beta-cutoff, therefore update killer for this level
//                    killers[level] = moves[i].HashKey;
//                    break;
//                }

             */
/*
            if (alpha <= score && score <= beta && score >= maxScore) {
                maxScore = score;
                bestMove = make_pair(piece, make_pair(rowTo, colTo));
            }

            alpha = max (alpha, maxScore);

            if(alpha >= beta) {
                break;
            }*/

            if (score > maxScore) {
                maxScore = score;
                bestMove = make_pair(piece, make_pair(rowTo, colTo));
                existMoves = true;
            }

            if (maxScore > alpha) {
                alpha = maxScore;
            }

            if (maxScore >= beta) {
                break;
            }

        }
    }


    return make_pair(maxScore, bestMove);
}