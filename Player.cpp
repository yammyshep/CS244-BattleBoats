#include "Player.h"

Player::Player(PlayerType type) {
    this->type = type;
}

Player::~Player() {
    //TODO: Implement
}

void Player::playTurn() {
    //TODO: Implement
}

ostream &operator<<(ostream &out, Player &p) {
    if (p.getType() == HUMAN_A) {
        out << "Player A";
    } else if (p.getType() == HUMAN_B) {
        out << "Player B";
    } else if (p.getType() == COMPUTER) {
        out << "CPU";
    } else {
        out << "ERR";
    }
    return out;
}

PlayerType Player::getType() const {
    return type;
}
