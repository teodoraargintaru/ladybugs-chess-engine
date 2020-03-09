#include "engine.h"

int main(){
    char command[78];
    string input;

    /*initializeBoard();
    printBoard();
    printMap();*/


    readFromInput(input); //xboard
    cin.rdbuf()->in_avail();
    readFromInput(input); //protover
    cout<<"feature sigint=0 usermove=1\n"; // force engine to send usermove before a move

    forever{
        cout.setf(ios::unitbuf);
        cin.rdbuf()->in_avail();

        readFromInput(input);
        strcpy(command, input.c_str());
        // TODO: clock

        if(strcmp(command, "new") == 0){
            initializeBoard();
            colorToMove = WHITE;
            engineColor = BLACK;
            //TODO : Associate the engine's clock with Black and the opponent's clock with White. Reset clocks *
        } else if(strcmp(command, "white") == 0){
            colorToMove = WHITE;
            engineColor = BLACK;
            force = false;
            applyStrategy();
        } else if(strcmp(command, "black") == 0){
            colorToMove = BLACK;
            engineColor = WHITE;
            force = false;
            applyStrategy();
        } else if(strcmp(command, "quit") == 0){
            break;
        } else if(strcmp(command, "force") == 0){
            engineColor = NONE;
            force = true;
            //TODO: Stop clocks. The engine should check that moves received in force
            // mode are legal and made in the proper turn, but should not think, ponder, or make moves of its own.
        } else if(strcmp(command, "go") == 0){
            engineColor = colorToMove;
            force = false;
            //TODO:  Associate the engine's clock with the color that is on move, the opponent's clock with the
            // color that is not on move. Start the engine's clock. Start thinking and eventually make a move.
        } else if(strstr(command, "usermove") != nullptr){
            char move[5]; // stores move as fromTo
            strcpy(move, command + 9);
            markMoveOnBoard(move);

            if(force){
                colorToMove = colorToMove == WHITE ? BLACK : WHITE;
            }

            applyStrategy();
        } else continue;
    }
    return 0;
}