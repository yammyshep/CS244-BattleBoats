#include <iostream>
#include <string>

#include "BattleBoatGame.h"
#include "Options.cpp"
#include "Player.h"

using namespace std;

BattleBoatGame::BattleBoatGame() {
    board = new BattleBoatBoard(Options::PLAYER_COUNT, Options::BOARD_SIZE_X, Options::BOARD_SIZE_Y);
    turnOrder[0] = new Player(HUMAN_A);
    turnOrder[1] = new Player(HUMAN_B);
}

BattleBoatGame::~BattleBoatGame() {
    delete board;
    delete turnOrder[0];
    delete turnOrder[1];

}

void BattleBoatGame::playGame() {
    setupGame();

    bool gameRunning{true};
    int turn{0};
    while (gameRunning) {

        runRound();

        // Exit conditions: a player has won
        Player* winner = board->getWinner(turnOrder);
        if (winner != nullptr) {
            gameRunning = false;
            cout << *board << endl;
            cout << *winner << " has won the game!" << endl;
        }
    }
}

/**
 * Setup helper function; prompts and places all boats for a given player
 * @param p player to place boats for
 */
void BattleBoatGame::placeBoats(Player& p) {
    int boatSizes[] = OPTION_DEF_BOAT_SIZES;
    for (int i = 0; i < Options::BOAT_COUNT; i++) { // For each of the boat sized defined in Options.cpp
        int size = boatSizes[i];
        bool placing{true};
        while (placing) { // Loop until boat placed successfully
            board->privatizeFor(turnIndex);
            cout << *board << endl << endl;
            board->publicize();
            cout << p << ": Placing " << size << " unit boat. ";

            int* boatLocation = ir.promptAndResolveBoardLocation(); // prompts for a location to be entered
            boatLocation[0] = turnIndex; // move the board index to the current player's index (above function doesnt know this and will always output -1)
            Orientation boatOrientation = ir.promptAndResolveOrientation(); // prompts for the orientation of the boat
            Boat* b = new Boat(p, boatLocation, size, boatOrientation); // creat the boat object

            delete boatLocation; // Boat will copy location data, clean up in this scope
            boatLocation = nullptr;

            if (board->placeBoat(*b)) { // break loop if boat was placed
                placing = false;
                p.addBoat(*b); // Pass a pointer to the player that owns the boat
            } else { cout << "Location Invalid!" << endl; } // otherwise, scream at the user and loop again

            clearTerm();
        }
    }

    board->privatizeFor(turnIndex);
    cout << *board << endl << endl;
    board->publicize();

    cout << "Press enter once device is passed to " << *turnOrder[!turnIndex] << ".";
    string trash;
    getline(cin, trash);

    clearTerm();
}

void BattleBoatGame::setupGame() {
    clearTerm();
    cout << "Welcome to BattleBoats!" << endl;
    for (turnIndex = 0; turnIndex < 2; turnIndex++) { // For each player
        placeBoats(*turnOrder[turnIndex]);
    }
    turnIndex = 0;
}

void BattleBoatGame::runRound() {
    for (turnIndex = 0; turnIndex < 2; turnIndex++) { // For each player
        board->privatizeFor(turnIndex);
        cout << *board << endl << endl;
        board->publicize();

        cout << "Your turn, " << *turnOrder[turnIndex] << endl;

        int* loc{nullptr};
        bool placed{false};

        TurnAction ta = ir.promptAndResolveTurnAction(); // Prompt player for a turn action
        switch (ta) {
            case SKIP:
                cout << "OK. Turn skipped." << endl;
                continue;
                break;
            case STRIKE:
                while(!placed) {
                    loc = ir.promptAndResolveBoardLocation(); // Get the location on board
                    loc[0] = !turnIndex; // move the board index to the opposing player's index (above function doesnt know this and will always output -1)
                    Strike* strike = new Strike(*turnOrder[turnIndex], loc);
                    delete loc;
                    placed = board->placeStrike(*strike);
                    if (!placed) {
                        cout << "Could not place a strike at that location! Try again!" << endl;
                    }
                }

                break;
            case SPECIAL:
                // Special
                while (!placed) {
                    loc = ir.promptAndResolveBoardLocation(); // Get the location on board
                    placed = true;
                    cerr << "NOT IMPLEMENTED LMAO" << endl; //TODO
                }
                break;
            default:
                // Should never be reached assuming InputResolver is sanitizing inputs properly
                cerr << "error in turnaction switch overload!" << endl;
        }

        clearTerm();

        board->privatizeFor(turnIndex);
        cout << *board << endl << endl;
        board->publicize();

        cout << "Press enter once device is passed to " << *turnOrder[!turnIndex] << ".";
        string trash;
        getline(cin, trash);

        clearTerm();
    }
    turnIndex = 0;
}

/**
 * Fancy feature: clear the console window
 * On linux, this is accomplished by printing some escape chars
 * On windows, however, its *slightly* less elegant
 */
void BattleBoatGame::clearTerm() {
    #ifdef __linux // Macro will be defined when compiled for linux
    // This amalgamation of escape codes will, *on linux*, and in most terminals, wipe the console.
    // doing the same on Windows requires libraries and stuff
    cout << "\x1B[2J\x1B[H";
    #else
    for (int i = 0; i < 50; i++) {
        cout << endl;
    }
    #endif
}
