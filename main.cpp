#include "engine.h"

void printAttacked(int color) {
    for(int i = 1; i <= 8; i++) {
        for(int j = 1; j <= 8; j++) {
            printf("%2d ", attacked[color - 1][i][j].size());
        }
        cout<<endl;
    }
}
int main(){
    char command[78];
    string input;

    cout.setf(ios::unitbuf);
    cin.rdbuf()->in_avail();

    readFromInput(input); //xboard
    cin.rdbuf()->in_avail();
    readFromInput(input); //protover
    //force engine to send usermove before a move
    cout<<"feature sigint=0 usermove=1 san=0\n";
    cout.flush();

    forever {
            cout.setf(ios::unitbuf);
            cin.rdbuf()->in_avail();

            readFromInput(input);
            strcpy(command, input.c_str());

            if(strcmp(command, "new") == 0){
                initializeBoard();

                unordered_map<int, vector< pair<int, int> > > dummy;
                markAttacked(dummy, WHITE);
                dummy.clear();
                markAttacked(dummy, BLACK);

                cout<<"WHITE "<< endl;
                printAttacked(WHITE);
                cout<<"BLACK"<<endl;
                printAttacked(BLACK);

                whiteKingMoved = false;
                blackKingMoved = false;
                whiteRookLeftMoved = false;
                whiteRookRightMoved = false;
                blackRookLeftMoved = false;
                blackRookRightMoved = false;
                promotedWhite = 0;
                promotedBlack = 0;
                promotionIsSet = false;
                pawn2moves = EMPTY;
                enPassant = false;

                colorToMove = WHITE;
                engineColor = BLACK;
            } else if(strcmp(command, "white") == 0){
                colorToMove = WHITE;
                engineColor = BLACK;
                force = false;
            } else if(strcmp(command, "black") == 0){
                colorToMove = BLACK;
                engineColor = WHITE;
                force = false;
            } else if(strcmp(command, "quit") == 0){
                break;
            } else if(strcmp(command, "force") == 0){
                engineColor = NONE;
                force = true;
            } else if(strcmp(command, "go") == 0){
                engineColor = colorToMove;
                force = false;
                applyStrategy();
            } else if(strstr(command, "usermove") != NULL){
                char move[5]; // stores move as fromTo
                strcpy(move, command + 9);
                markMoveOnBoard(move);

                cout<<"AFTER MOVE"<<endl;
                cout<<"WHITE "<< endl;
                printAttacked(WHITE);
                cout<<"BLACK"<<endl;
                printAttacked(BLACK);

                if(force){
                    colorToMove = colorToMove == WHITE ? BLACK : WHITE;
                }
                applyStrategy();
            } else continue;
    }
    return 0;
}