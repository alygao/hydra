#ifndef __GUI_DISPLAY_H__
#define __DISPLAY_H__

#include "gameDisplay.h"
#include "x11Window.h"
#include "pokerCardPixelLibrary.h"

class GUIDisplay : public GameDisplay, public X11Window {

    mutable GameInfo previousGameInfo;

    int firstHeadSeqNoOnScreen = 0;
    int firstPlayerSeqNoOnScreen = 0;

    virtual void displayHeads() const noexcept override;
    virtual void displayPlayers() const noexcept override;
    virtual void displayWinner() const noexcept override;
    virtual std::string promptForSuit ( std::string reason );

    void displayHeadsNextPage();
    void displayHeadsPriorPage();
    void displayPlayersNextPage();
    void displayPlayersPriorPage();
    // Process Pagination actions for displaying heads and players
    // Returns: true for valid actions; false for invalid actions
    bool processPaginationAction ( std::string action );
    std::string inputString ( const std::string & question = "" ) const;
    virtual void displayAnswer ( const std::string & answer ) const;
    void clearMessage ( ) const;
    virtual void clearAnswer ( ) const override;
    void clearErrorMessage ( ) const;
    void displayErrorMessage ( const std::string & message ) const;

    void displayInstructions() const;
    void initPlayersDisplay ( ) const;
    void initHeadsDisplay ( ) const;
    void initPlayerDisplay ( int index ) const;
    void initHeadDisplay ( int index ) const;

    bool hasHeadInfoChanged ( int index ) const;
    bool hasPlayerInfoChanged ( int index ) const;

    virtual void refreshDisplay() const override;
    mutable std::string currentMessage = ""; // for re-display purpose (set in displayMessage(), reset in clearMessage())
    mutable std::string currentErrorMessage = ""; // for re-display purpose (set in displayErrorMessage(), reset in clearErrorMessage())

    virtual void drawImageIfNotExist ( const std::string & imageName, int x, int y ) const override;
    void drawImage ( int x, int y, std::shared_ptr<PokerCardPixel> cardPixel, int ratio, int colour ) const;
    void drawImage ( int x, int y, std::string imageKey, int ratio, int colour = Black ) const;
    
    int level;  // 2 - use images for cards and logo, 2 - use shapes and text for displaying cards
    PokerCardPixelLibrary pokerCardPixelLibrary;
    

  public:
    GUIDisplay ( int level = 2);

    virtual int inputNumPlayers ( ) override;
    virtual void inputToAcceptATurn ( int playerId, bool isComputerPlayer = false ) override;
    virtual int inputHeadId ( int playerId, std::string cardDisplayName, bool previousAnswerHasError = false, int preAnsweredHeadNo = -1 ) override;
    virtual int inputJokerValue ( int preAnsweredJokerValue = 0 ) override;
    virtual std::string promptForCardValue ( std::string reason ) override;
    virtual void displayMessage ( const std::string & message ) const override;
    virtual void acknowledge ( const std::string & message ) const override;

    virtual ~GUIDisplay ( ) override;
};

#endif
