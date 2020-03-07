#include <fstream>
#include <iostream>
#include <string.h>
#include <time.h>

using namespace std;

#define WHITE    1
#define BLACK    2
#define NONE     0
#define ANALYZE  3
#define forever while(1)

void readFromInput(string &input){
    getline(cin, input);
    if(input.size() > 77){
        exit(0);
    }
}

// TODO: map the board and the pieces in order to check legal moves

int main(){
    int colorToMove = WHITE, engineColor = NONE;
    char command[78];
    string input;
    ofstream out("fisier.out");

    forever{
        cout.setf(ios::unitbuf);
        cin.rdbuf()->in_avail();

        readFromInput(input);
        strcpy(command, input.c_str());
        // TODO: clock

        if(strcmp(command, "xboard") == 0){
            continue;
            //TODO??
        } else if(strcmp(command, "new") == 0){
                colorToMove = WHITE;
                engineColor = BLACK;
                cout<<"feature sigint=0 usermove=1\n"; // force engine to send usermove before a move
            //TODO : Associate the engine's clock with Black and the opponent's clock with White. Reset clocks *
        } else if(strcmp(command, "white") == 0){
            colorToMove = WHITE;
        } else if(strcmp(command, "black") == 0){
            colorToMove = BLACK;
        } else if(strcmp(command, "quit") == 0){
            break;
        } else if(strcmp(command, "resign") == 0){

        } else if(strcmp(command, "force") == 0){
            engineColor = NONE;
            //TODO: Stop clocks. The engine should check that moves received in force
            // mode are legal and made in the proper turn, but should not think, ponder, or make moves of its own.
        } else if(strcmp(command, "go") == 0){
            engineColor = colorToMove;
            //TODO:  Associate the engine's clock with the color that is on move, the opponent's clock with the
            // color that is not on move. Start the engine's clock. Start thinking and eventually make a move.
        } else if(strstr(command, "usermove") != nullptr){
            char move[70]; // stores move as fromTo
            strcpy(move, command + 9);

            colorToMove = BLACK;
            //dummy move to check if this works
            cout<<"move e7e5\n";

            //TODO: move to position, check if move is valid
        } else continue;
    }
    return 0;
}