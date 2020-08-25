#ifndef __TEXT_DISPLAY_H__
#define __TEXT_DISPLAY_H__

#include "gameDisplay.h"

class TextDisplay : public GameDisplay {
    std::istream & in;
    std::ostream & out;
    std::ostream & err;
    virtual void displayHeads() const noexcept override;
    virtual void displayPlayers() const noexcept override;
    virtual void displayWinner() const noexcept override;
    virtual std::string promptForSuit ( std::string reason );

  public:
    TextDisplay ( std::istream & in, std::ostream & out, std::ostream & err );

    virtual int inputNumPlayers ( ) override;
    virtual void inputToAcceptATurn ( int playerId, bool isComputerPlayer = false ) override;
    virtual int inputHeadId ( int playerId, std::string cardDisplayName, bool previousAnswerHasError = false, int preAnsweredHeadNo = -1 ) override;
    virtual int inputJokerValue ( int preAnsweredJokerValue = 0 ) override;
    virtual std::string promptForCardValue (std::string reason ) override;
    virtual void displayMessage ( const std::string & message ) const override;
    virtual void acknowledge ( const std::string & message ) const override;

    virtual ~TextDisplay ( );
};

#endif
