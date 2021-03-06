#pragma once

#include "BattleBoatBoard.h"
#include "Player.h"
#include "InputResolver.h"

class BattleBoatGame {
public:
	BattleBoatGame();
	~BattleBoatGame();
	void playGame();
private:
	BattleBoatBoard* board;
	Player* turnOrder[2];
    InputResolver ir{};
	int turnIndex;
	void runTurnForPlayer(Player);
	void runRound();
    void setupGame();

    void placeBoats(Player &p);

    static void clearTerm();
};
