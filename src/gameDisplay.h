#ifndef __GAME_DISPLAY_H__
#define __GAME_DISPLAY_H__

#include <iostream>
#include <istream>
#include <map>
#include <memory>
#include "observer.h"
#include "gameInfo.h"

const std::string PROMPT_NUM_OF_PLAYERS = "How many players?";
const std::string PROMPT_START_A_TURN_1 = "Player ";
const std::string PROMPT_START_A_TURN_2 = ", it is your turn. ('Q' to quit, Enter to continue)";
const std::string PROMPT_CHOOSE_HEAD_1 = "Player ";
const std::string PROMPT_CHOOSE_HEAD_2 = ", you are holding a ";
const std::string PROMPT_CHOOSE_HEAD_3 = ". Your move?";
const std::string PROMPT_JOKER_VALUE = "Joker value?";
const std::string PROMPT_FOR_TESTING_1 = "Input for ";
const std::string PROMPT_FOR_TESTING_2 = " in testing mode: ";
const std::string PROMPT_CARD_VALUE = "Card value?";
const std::string PROMPT_SUIT = "Suit?"; 

const std::string ERROR_MSG_INVALID_INPUT = "Invalid input, please try again.";

class PlayerQuitException { };
class PlayerTerminateException { };

class GameDisplay : public Observer {
  private:
    virtual void displayHeads() const noexcept = 0;
    virtual void displayPlayers() const noexcept = 0;
    virtual void displayWinner() const noexcept = 0;

  protected:
    GameInfo gameInfo;

    // Converts card face value in integer to string value.
    std::string convertFaceValueToString ( int faceValue );

    // Converts card face value in string to int value.
    // Returns 1 - 13 for valid value; -1 otherwise.
    int convertFaceValueToInt ( std::string faceValueStr );

  public:
    GameDisplay ( );
    virtual ~GameDisplay ( );

    virtual void notify ( GameSubject & gameSubject ) override;

    virtual void displayHeadsAndPlayers ( ) const noexcept;
    virtual int inputNumPlayers ( ) = 0;
    virtual void inputToAcceptATurn ( int player, bool isComputerPlayer = false ) = 0;
    virtual int inputHeadId ( int playerId, std::string cardDisplayName, bool previousAnswerHasError = false, int preAnsweredHeadNo = -1 ) = 0;
    virtual int inputJokerValue ( int preAnsweredJokerValue = 0 ) = 0;
    
    // Asks card value in testing mode, if the card value is not Joker, asks for suit.
    // Returns a string in format of "S#", S - H/S/D/C/J (J - Joker), # - 1-13
    virtual std::string promptForCardValue ( std::string reason ) = 0; // asking card value in testing mode, if the card value is not Joker, ask for suit
    virtual void displayMessage ( const std::string & message ) const = 0;
    virtual void acknowledge ( const std::string & message ) const = 0;
};

#endif
