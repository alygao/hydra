#include "textDisplay.h"
#include "util.h"

//------------------------ TextDisplay Implementation ----------------------

TextDisplay::TextDisplay ( std::istream & in, std::ostream & out, std::ostream & err ) : in { in }, out { out }, err { err } { };

void TextDisplay::displayHeads() const noexcept {
    out << "Heads:" << std::endl;
    for ( auto head : gameInfo.headsInfo ) {
        out << head.headId << ": " << head.topCard << " (" << head.numOfCards << ")" << std::endl;
    }
    out << std::endl;
}

void TextDisplay::displayPlayers() const noexcept {
    out << "Players:" << std::endl;
    for ( auto player : gameInfo.playersInfo ) {
        if ( player.isComputerPlayer ) {
            out << "Computer ";
        }
        out << "Player " << player.playerId << ": " << player.numOfDrawPile + player.numOfDiscardPile
            << " (" << player.numOfDrawPile << " draw, " << player.numOfDiscardPile << " discard)";
        if ( player.isTurn ) {
            out << " + 1 in hand, " << player.numOfRemainingCards << " remaining, "
                << player.numOfReservedCards << " in reserve";
        }
        out << std::endl;
    }
    out << std::endl;
}

void TextDisplay::displayWinner() const noexcept {
    out << "Player " << gameInfo.winningPlayerId << " wins!" << std::endl;
}

int TextDisplay::inputNumPlayers ( ) {
    in.exceptions ( std::ios::eofbit | std::ios::failbit );
    int numOfPlayers = 0;
    while ( numOfPlayers <= 1 ) {
        out << grammarCorrection ( PROMPT_NUM_OF_PLAYERS ) << std::endl;
        try {
            std::string answer;
            getline ( in, answer );
            numOfPlayers = std::stoi ( answer );
        } catch (std::ios::failure &) {
            if (in.eof()) { throw PlayerTerminateException{}; }  // allows player to quit at the very beginning of the game.
            in.ignore();
            in.clear();
            err << ERROR_MSG_INVALID_INPUT << std::endl;
        } catch (std::invalid_argument &) {
            err << ERROR_MSG_INVALID_INPUT << std::endl;
        }
    }
    return numOfPlayers;
}

void TextDisplay::inputToAcceptATurn ( int playerId, bool isComputerPlayer ) {
    in.exceptions ( std::ios::eofbit | std::ios::failbit );
    std::string promptStr = grammarCorrection ( PROMPT_START_A_TURN_1 + std::to_string(playerId) + PROMPT_START_A_TURN_2 );
    if ( isComputerPlayer ) {
        out << "Computer " << promptStr << std::endl;
        out << "Computer player has accepted" << std::endl;
        return;
    }
    out << promptStr << std::endl;
    try {
        std::string answer;
        std::getline ( in, answer );
        if ( answer == "q" || answer == "Q" ) {
            throw PlayerQuitException{};
        }
    } catch (std::ios::failure &) {
        if (in.eof()) { throw PlayerTerminateException{}; }
        in.clear();
        in.ignore();
        err << ERROR_MSG_INVALID_INPUT << std::endl;
    }
}

int TextDisplay::inputHeadId ( int playerId, std::string cardDisplayName, bool previousAnswerHasError, int preAnsweredHeadNo ) { // TODO: consider to pass the validation function from Game to Display
    if ( previousAnswerHasError ) err << ERROR_MSG_INVALID_INPUT << std::endl;
    in.exceptions ( std::ios::eofbit | std::ios::failbit );
    int headNo = -1;
    std::string promptStr = grammarCorrection ( PROMPT_CHOOSE_HEAD_1 + std::to_string(playerId) + PROMPT_CHOOSE_HEAD_2 + cardDisplayName + PROMPT_CHOOSE_HEAD_3 );
    if ( preAnsweredHeadNo >= 0 ) {
        out << "Computer " << promptStr << std::endl;
        out << preAnsweredHeadNo << std::endl;
        return preAnsweredHeadNo;
    }
    while ( headNo < 0 ) {
        out << promptStr << std::endl;
        try {
            std::string answer;
            getline ( in, answer );
            headNo = std::stoi ( answer );
        } catch (std::ios::failure &) {
            if (in.eof()) { throw PlayerTerminateException{}; }
            in.clear();
            in.ignore();
            err << ERROR_MSG_INVALID_INPUT << std::endl;
        } catch (std::invalid_argument &) {
            err << ERROR_MSG_INVALID_INPUT << std::endl;
        }
    }
    return headNo;
}

int TextDisplay::inputJokerValue ( int preAnsweredJokerValue ) {
    in.exceptions ( std::ios::eofbit | std::ios::failbit );
    std::string jokerRank;
    std::string prompt = grammarCorrection ( PROMPT_JOKER_VALUE );
    if ( preAnsweredJokerValue > 0 ) {
        out << prompt << std::endl;
        out << convertFaceValueToString ( preAnsweredJokerValue ) << std::endl;
        return preAnsweredJokerValue;
    }
    while ( true ) {
        out << prompt << std::endl;
        try {
            std::getline ( in, jokerRank );
            jokerRank = trim ( jokerRank );
            int faceValue = convertFaceValueToInt ( jokerRank );
            if ( faceValue <= 0 ) {
                err << ERROR_MSG_INVALID_INPUT << std::endl;
            } else {
                return faceValue;
            }
        } catch (std::ios::failure &) {
            if (in.eof()) { throw PlayerTerminateException{}; }
            in.clear();
            in.ignore();
            err << ERROR_MSG_INVALID_INPUT << std::endl;
        }
    }
}

// Asks card value in testing mode, if the card value is not Joker, asks for suit.
// Returns a string in format of "S#", S - H/S/D/C/J (J - Joker), # - 1-13
std::string TextDisplay::promptForCardValue ( std::string reason ) {
    in.exceptions ( std::ios::eofbit | std::ios::failbit );
    std::string cardRank;
    out << PROMPT_FOR_TESTING_1 << reason << PROMPT_FOR_TESTING_2 << std::endl;    
    while ( true ) {
        out << grammarCorrection ( PROMPT_CARD_VALUE ) << std::endl;
        try {
            int faceValue = 0;
            std::getline ( in, cardRank );
            cardRank = trim ( cardRank );
            faceValue = convertFaceValueToInt ( cardRank );
            if ( faceValue < 0 ) {
                err << ERROR_MSG_INVALID_INPUT << std::endl;
                continue;
            } else if ( faceValue == 0 ) {  // Joker
                return "J";
            }
            std::string suit = promptForSuit(reason);
            return suit + std::to_string ( faceValue );
        } catch (std::ios::failure &) {
            if (in.eof()) { throw PlayerTerminateException{}; }
            in.clear();
            in.ignore();
            err << ERROR_MSG_INVALID_INPUT << std::endl;
        }
    }
}

std::string TextDisplay::promptForSuit ( std::string reason ) {
    in.exceptions ( std::ios::eofbit | std::ios::failbit );
    std::string suit;
    while ( true ) {
        out << grammarCorrection ( PROMPT_SUIT ) << std::endl;
        try {
            std::getline ( in, suit );
            suit = trim ( suit );
            suit = to_uppercase ( suit );
            if ( suit == "H"  || suit == "S" || suit == "D" || suit == "C" ) {
                return suit;
            } else {
                err << ERROR_MSG_INVALID_INPUT << std::endl;
            }
        } catch (std::ios::failure &) {
            if (in.eof()) { throw PlayerTerminateException{}; }
            in.clear();
            in.ignore();
            err << ERROR_MSG_INVALID_INPUT << std::endl;
        }
    }
}

void TextDisplay::displayMessage ( const std::string & message ) const {
    out << message << std::endl;
}

void TextDisplay::acknowledge ( const std::string & message ) const {
    displayMessage ( message ); // for Textdiaply, no acknowledgement is required.
}

TextDisplay::~TextDisplay ( ) { }
