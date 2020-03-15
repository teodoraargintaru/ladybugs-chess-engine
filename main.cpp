#include "engine.h"

int main(){
    char command[78];
    string input;

    cout.setf(ios::unitbuf);
    cin.rdbuf()->in_avail();

    readFromInput(input); //xboard
    cin.rdbuf()->in_avail();
    readFromInput(input); //protover
    //force engine to send usermove before a move
    cout<<"feature sigint=0 usermove=1\n";
    cout.flush();

    forever{
        cout.setf(ios::unitbuf);
        cin.rdbuf()->in_avail();

        readFromInput(input);
        strcpy(command, input.c_str());

        if(strcmp(command, "new") == 0){
            initializeBoard();
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
            if(force){
                colorToMove = colorToMove == WHITE ? BLACK : WHITE;
            }
            applyStrategy();
        } else continue;
    }
    return 0;
}