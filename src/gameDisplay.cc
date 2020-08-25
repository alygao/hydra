#include "gameDisplay.h"
#include "gameSubject.h"
#include "pokerCardPixel.h"
#include "util.h"

//------------------------ Game Display Implementation ----------------------

static const std::string CARD_FACE_VALUE_FOR_DISPLAY = "A234567890JQK";

GameDisplay::GameDisplay ( ) { }
GameDisplay::~GameDisplay ( ) { }

void GameDisplay::notify ( GameSubject & gameSubject ) {
    gameInfo = gameSubject.getGameInfo();
}

void GameDisplay::displayHeadsAndPlayers ( ) const noexcept {
    if ( gameInfo.winningPlayerId == 0 ) {
        displayHeads();
        displayPlayers();
    } else {
        displayWinner();
    }
}

// Converts card face value in integer to string value.
std::string GameDisplay::convertFaceValueToString ( int faceValue ) {
    if ( faceValue < 0 || faceValue > 12 ) {
        return "";
    } else if ( faceValue == 10 ) {
        return "10";
    } else {
        return std::string { CARD_FACE_VALUE_FOR_DISPLAY[faceValue-1] };
    }
}

// Converts card face value in string to int value.
// Returns 1 - 13 for regular card value; 0 - Joker; -1 invalid value.
int GameDisplay::convertFaceValueToInt ( std::string faceValueStr ) {
    faceValueStr = to_uppercase ( faceValueStr );
    if ( faceValueStr == "A" ) {
        return 1;
    } else if ( faceValueStr == "J" ) {
        return 11;
    } else if ( faceValueStr == "Q" ) {
        return 12;
    } else if ( faceValueStr == "K" ) {
        return 13;
    } else if ( faceValueStr == "JOKER" ) {
        return 0;
    } else {
        // only accepts 1/A for Ace, 11/J for Jack, 12/Q for Queen, 13/K for King
        try {
            int faceValue = std::stoi ( faceValueStr );
            if ( faceValue < 1 || faceValue > 13 ) {
                return -1;
            }
            return faceValue;
        } catch ( std::exception & e ) {
            return -1;
        }
    }
}
