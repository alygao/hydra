#include "guiDisplay.h"
// #include "gameSubject.h"
#include "pokerCardPixel.h"
#include "util.h"

//------------------------ GUIDisplay Implementation ----------------------

static const int SCREEN_WIDTH = 1600;
static const int SCREEN_HEIGHT = 900;

static const int CARD_WIDTH = 60;
static const int CARD_HEIGHT = 90;

static const int PLAYER_FRAME_INIT_LEFT_X = 15;
static const int PLAYER_FRAME_INIT_RIGHT_X = 1315;
static const int PLAYER_FRAME_INIT_Y = 15;
static const int PLAYER_FRAME_WIDTH = 260;
static const int PLAYER_FRAME_HEIGHT = 180;
static const int PLAYER_FRAME_GAP = 10;

static const int PLAYER_PILES_OFFSET_X = 5;
static const int PLAYER_PILES_OFFSET_Y = 5;
static const int PLAYER_PILES_GAP_X = 4;
static const int PLAYER_PILE_NAME_OFFSET_X = 5;
static const int PLAYER_PILE_NAME_OFFSET_Y = 15;
static const int PLAYER_NAME_OFFSET_X = 85;
static const int PLAYER_NAME_OFFSET_Y = 135;
static const int PLAYER_TURN_MSG_OFFSET_X = 50;
static const int PLAYER_TURN_MSG_OFFSET_Y = 160;


static const int NUM_OF_PLAYERS_ON_SCREEN = 8;
static const int NUM_OF_HEAD_ROWS_ON_SCREEN = 4;
static const int NUM_OF_HEAD_COLUMNS_ON_SCREEN = 9;

static const int HEAD_FRAME_INIT_X = 360;
static const int HEAD_FRAME_INIT_Y = 40;
static const int HEAD_FRAME_WIDTH = 60;
static const int HEAD_FRAME_HEIGHT = 90;
static const int HEAD_FRAME_GAP_X = 40;
static const int HEAD_FRAME_GAP_Y = 60;

static const int COMMAND_SECTION_POS_Y = 780;
static const int ERR_MSG_POS_X = 50;
static const int ERR_MSG_POS_Y = 820;
static const int ERR_MSG_WIDTH = 1000;
static const int ERR_MSG_HEIGHT = 30;
static const int MESSAGE_POS_X = 50;
static const int MESSAGE_POS_Y = 850;
static const int MESSAGE_WIDTH = 1000;
static const int MESSAGE_HEIGHT = 30;
static const int INPUT_POS_X = 50;
static const int INPUT_POS_Y = 880;
static const int INPUT_WIDTH = 1000;
static const int INPUT_HEIGHT = 30;

static const int INSTRUCTION_POS_X = 1290;
static const int INSTRUCTION_POS_Y = 805;
static const int INSTRUCTION_WIDTH = 260;
static const int INSTRUCTION_HEIGHT = 93;

static const std::string IMAGE_NAME_LOGO = "hydra_logo";
static const std::string IMAGE_NAME_CARD_BACK = "card_back";
static const std::string IMAGE_NAME_JOKER = "Joker";

static const int LOGO_X = 425;
static const int LOGO_Y = 350;
static const int LOGO_WIDTH = 654;
static const int LOGO_HEIGHT = 105;

GUIDisplay::GUIDisplay ( int level ) : level { level } {
    refreshDisplay();
}

GUIDisplay::~GUIDisplay ( ) { }

void GUIDisplay::refreshDisplay ( ) const {

    // Background
    fillRectangle ( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GameBackgroundColour );

    // lines
    drawRectangle ( PLAYER_FRAME_INIT_LEFT_X + PLAYER_FRAME_WIDTH + 15, 0, 2, COMMAND_SECTION_POS_Y, LineColour );
    drawRectangle ( PLAYER_FRAME_INIT_RIGHT_X - 15, 0, 2, COMMAND_SECTION_POS_Y, LineColour );
    drawRectangle ( 0, COMMAND_SECTION_POS_Y, SCREEN_WIDTH, 2, LineColour );

    // player frame lines
    initPlayersDisplay();

    // heads
    // initHeadsDisplay();

    // Frame
    fillRectangle ( 0, 2, SCREEN_WIDTH, 3, FrameColour );
    fillRectangle ( 0, SCREEN_HEIGHT - 2, SCREEN_WIDTH, 2, FrameColour );
    fillRectangle ( 0, 0, 2, SCREEN_HEIGHT, FrameColour );
    fillRectangle ( SCREEN_WIDTH - 2, 0, 2, SCREEN_HEIGHT, FrameColour );

    // logo
    displayImage ( IMAGE_NAME_LOGO, LOGO_X, LOGO_Y );

    previousGameInfo.headsInfo.clear();
    previousGameInfo.playersInfo.clear();

    displayHeadsAndPlayers();

    // processing refresh (triggerred by the X11 Window Expose event)
    if ( currentMessage != "" ) {
        std::string msg = currentMessage;
        displayMessage ( msg );
    }
    if ( currentErrorMessage != "" ) {
        std::string msg = currentErrorMessage;
        displayErrorMessage ( msg );
    }

    displayInstructions();
}

void GUIDisplay::displayInstructions() const {
    fillRectangle ( INSTRUCTION_POS_X, INSTRUCTION_POS_Y, INSTRUCTION_WIDTH, INSTRUCTION_HEIGHT, GameBackgroundColour );
    drawString ( INSTRUCTION_POS_X, INSTRUCTION_POS_Y, "Paginating Headrs/Players:" );
    drawString ( INSTRUCTION_POS_X, INSTRUCTION_POS_Y + 25, "Heads Page-Up: \"prior heads\" or \"ph\"" );
    drawString ( INSTRUCTION_POS_X, INSTRUCTION_POS_Y + 43, "Heads Page-Down: \"next heads\" or \"nh\"" );
    drawString ( INSTRUCTION_POS_X, INSTRUCTION_POS_Y + 61, "Players Page-Up: \"prior players\" or \"pp\"" );
    drawString ( INSTRUCTION_POS_X, INSTRUCTION_POS_Y + 79, "Players Page-Down: \"next players\" or \"np\"" );
}

void GUIDisplay::initPlayersDisplay ( ) const {
    for ( int i = 0; i < NUM_OF_PLAYERS_ON_SCREEN; i ++ ) {
        initPlayerDisplay ( i );
    }
}

void GUIDisplay::initHeadsDisplay ( ) const {
    for ( int i = 0; i < NUM_OF_HEAD_ROWS_ON_SCREEN * NUM_OF_HEAD_COLUMNS_ON_SCREEN; i ++ ) {
        initHeadDisplay ( i );
    }
}

void GUIDisplay::initPlayerDisplay ( int index ) const {
    int row = index / 2;
    int column = index % 2;
    int x = column == 0 ? PLAYER_FRAME_INIT_LEFT_X : PLAYER_FRAME_INIT_RIGHT_X;
    int y = PLAYER_FRAME_INIT_Y + ( PLAYER_FRAME_HEIGHT + PLAYER_FRAME_GAP ) * row;
    fillRectangle ( x, y, PLAYER_FRAME_WIDTH, PLAYER_FRAME_HEIGHT, GameBackgroundColour );
    drawRectangle ( x, y, PLAYER_FRAME_WIDTH, PLAYER_FRAME_HEIGHT, Grey );
}

void GUIDisplay::initHeadDisplay ( int index ) const {
    int row = index / NUM_OF_HEAD_COLUMNS_ON_SCREEN;
    if ( row >= 2 ) row ++; // skip the row for logo
    int column = index % NUM_OF_HEAD_COLUMNS_ON_SCREEN;
    int x = HEAD_FRAME_INIT_X + column * ( HEAD_FRAME_WIDTH + HEAD_FRAME_GAP_X );
    int y = HEAD_FRAME_INIT_Y + row * ( HEAD_FRAME_HEIGHT + HEAD_FRAME_GAP_Y );
    fillRectangle ( x - 5, y, HEAD_FRAME_WIDTH + HEAD_FRAME_GAP_X - 5, HEAD_FRAME_HEIGHT + HEAD_FRAME_GAP_Y - 11, GameBackgroundColour );
}

void GUIDisplay::displayHeads() const noexcept {
    int headSeqNo = -1;
    int headDisplaySeqNo = 0;
    for ( auto head : gameInfo.headsInfo ) {
        headSeqNo ++;
        if ( headSeqNo >= firstHeadSeqNoOnScreen + NUM_OF_HEAD_ROWS_ON_SCREEN * NUM_OF_HEAD_COLUMNS_ON_SCREEN ) {
            break;
        }
        if ( headSeqNo < firstHeadSeqNoOnScreen ) {
            continue;
        }
        if ( hasHeadInfoChanged ( headDisplaySeqNo + firstHeadSeqNoOnScreen ) ) {
            initHeadDisplay ( headDisplaySeqNo );
            int x = (headDisplaySeqNo % NUM_OF_HEAD_COLUMNS_ON_SCREEN) * (HEAD_FRAME_WIDTH + HEAD_FRAME_GAP_X) + HEAD_FRAME_INIT_X;
            int row = (headDisplaySeqNo / NUM_OF_HEAD_COLUMNS_ON_SCREEN);
            if ( row >= 2 ) row ++; // skip the row for logo
            int y = row * (HEAD_FRAME_HEIGHT + HEAD_FRAME_GAP_Y) + HEAD_FRAME_INIT_Y;
            std::string topCardImageName = head.topCard;
            bool isJokerCard = false;
            std::string announcedFaceValue = "";
            if ( topCardImageName.size() > 0 && topCardImageName[topCardImageName.size()-1] == 'J' ) {
                isJokerCard = true;
                announcedFaceValue = topCardImageName.substr ( 0, topCardImageName.size() - 1 );
                topCardImageName = "Joker";
            }
            displayImage ( topCardImageName, x, y );
            drawString ( x - 3, y + HEAD_FRAME_HEIGHT + 20, "Head #" + std::to_string(head.headId) + " (" + std::to_string(head.numOfCards) + ")" );
            if ( isJokerCard ) {
                drawImage ( x + 45, y + 10, announcedFaceValue, 2, Red );
                drawImage ( x + 10, y + 70, announcedFaceValue, 2, Red );
            }
        }
        // no need to worry about to clean previous extra heads, as heads will never decrease (like players)
        headDisplaySeqNo ++;
    }
    previousGameInfo.headsInfo = gameInfo.headsInfo;
}

void GUIDisplay::displayPlayers() const noexcept {
     // draw player frames
    for ( int i = 0; i < NUM_OF_PLAYERS_ON_SCREEN; i ++ ) {
        int x = i % 2 == 0 ? PLAYER_FRAME_INIT_LEFT_X : PLAYER_FRAME_INIT_RIGHT_X;
        int y = PLAYER_FRAME_INIT_Y + ( i / 2 ) * ( PLAYER_FRAME_HEIGHT + PLAYER_FRAME_GAP );
        drawRectangle ( x, y, PLAYER_FRAME_WIDTH, PLAYER_FRAME_HEIGHT, Grey );
    }
    int playerSeqNo = -1;
    int playerDisplaySeqNo = 0;
    for ( auto player : gameInfo.playersInfo ) {
        playerSeqNo ++;
        if ( playerSeqNo >= firstPlayerSeqNoOnScreen + NUM_OF_PLAYERS_ON_SCREEN ) {
            break;
        }
        if ( playerSeqNo < firstPlayerSeqNoOnScreen ) {
            continue;
        }
        if ( hasPlayerInfoChanged ( playerDisplaySeqNo + firstPlayerSeqNoOnScreen ) ) {
            initPlayerDisplay ( playerDisplaySeqNo );
            int baseX = ( playerDisplaySeqNo % 2 == 0 ? PLAYER_FRAME_INIT_LEFT_X : PLAYER_FRAME_INIT_RIGHT_X ) + PLAYER_PILES_OFFSET_X;
            int baseY = PLAYER_FRAME_INIT_Y + (PLAYER_FRAME_HEIGHT + PLAYER_FRAME_GAP) * (playerDisplaySeqNo / 2) + PLAYER_PILES_OFFSET_Y;
            // draw pile
            if ( player.numOfDrawPile == 0 ) {
                drawRectangle ( baseX,
                                baseY,
                                CARD_WIDTH, CARD_HEIGHT, Grey );
            } else {
                displayImage ( "card_back", 
                                baseX,
                                baseY );
            }
            drawString ( baseX + PLAYER_PILE_NAME_OFFSET_X,
                        baseY + CARD_HEIGHT + PLAYER_PILE_NAME_OFFSET_Y,
                        "Draw(" + std::to_string(player.numOfDrawPile) + ")" );
            // discard pile
            if ( player.numOfDiscardPile == 0 ) {
                drawRectangle ( baseX + CARD_WIDTH + PLAYER_PILES_GAP_X,
                                baseY,
                                CARD_WIDTH, CARD_HEIGHT, Grey );
            } else {
                displayImage ( "card_back", 
                                baseX + CARD_WIDTH + PLAYER_PILES_GAP_X,
                                baseY );
            }
            drawString ( baseX + (CARD_WIDTH + PLAYER_PILES_GAP_X) + PLAYER_PILE_NAME_OFFSET_X - 5, // adjust 5 pixels left for discard pile.
                        baseY + CARD_HEIGHT + PLAYER_PILE_NAME_OFFSET_Y,
                        "Discard(" + std::to_string(player.numOfDiscardPile) + ")" );
            int colour = Yellow;
            if ( player.isTurn ) {
                colour = Green;
                // reserve
                if ( player.numOfReservedCards == 0 ) {
                    drawRectangle ( baseX + (CARD_WIDTH + PLAYER_PILES_GAP_X) * 2,
                                    baseY,
                                    CARD_WIDTH, CARD_HEIGHT, Grey );
                } else {
                    displayImage ( "card_back", 
                                    baseX + (CARD_WIDTH + PLAYER_PILES_GAP_X) * 2,
                                    baseY );
                }
                drawString ( baseX + (CARD_WIDTH + PLAYER_PILES_GAP_X) * 2 + PLAYER_PILE_NAME_OFFSET_X + 3, // adjust 3 pixels right for reserve
                            baseY + CARD_HEIGHT + PLAYER_PILE_NAME_OFFSET_Y,
                            "Reserve" );
                // card-in-hand
                std::string cardInHandImageName = player.cardInHand;
                if ( cardInHandImageName.size() > 0 && cardInHandImageName[cardInHandImageName.size()-1] == 'J' ) {
                    cardInHandImageName = "Joker";
                }
                displayImage ( cardInHandImageName, 
                                baseX + (CARD_WIDTH + PLAYER_PILES_GAP_X) * 3, 
                                baseY );
                drawString ( baseX + (CARD_WIDTH + PLAYER_PILES_GAP_X) * 3 + PLAYER_PILE_NAME_OFFSET_X + 2, // adjust 2 pixels right for in-hand
                            baseY + CARD_HEIGHT + PLAYER_PILE_NAME_OFFSET_Y, 
                            "in hand" );
                drawString ( baseX + PLAYER_TURN_MSG_OFFSET_X, 
                            baseY + PLAYER_TURN_MSG_OFFSET_Y, 
                            "is playing (" + std::to_string(player.numOfRemainingCards) + " remaining)",
                            colour);
            }
            // player name
            std::string playerName = "Player " + std::to_string(player.playerId) + " (" + std::to_string(player.numOfDrawPile + player.numOfDiscardPile) + ")";
            int xPosAdjustment = 0;
            if ( player.isComputerPlayer ) {
                playerName = "Computer " + playerName;
                xPosAdjustment = -20;
            }
            drawString ( baseX + PLAYER_NAME_OFFSET_X + xPosAdjustment, baseY + PLAYER_NAME_OFFSET_Y, playerName, colour );
        }
        playerDisplaySeqNo ++;
    }
     // clear removed players but left on the screen (happens when the player quits but no computer player is setup)
    playerSeqNo = -1;
    playerDisplaySeqNo = 0;
    int currentPlayersInfoSize = gameInfo.playersInfo.size();
    if ( previousGameInfo.playersInfo.size() > gameInfo.playersInfo.size() ) {
        for ( auto player : previousGameInfo.playersInfo ) {
            playerSeqNo ++;
            if ( playerSeqNo >= firstPlayerSeqNoOnScreen + NUM_OF_PLAYERS_ON_SCREEN ) {
                break;
            }
            if ( playerSeqNo < firstPlayerSeqNoOnScreen ) {
                continue;
            }
            if ( playerDisplaySeqNo + firstPlayerSeqNoOnScreen >= currentPlayersInfoSize ) {
                initPlayerDisplay ( playerDisplaySeqNo );
            }
            playerDisplaySeqNo ++;
        }
    }
    previousGameInfo.playersInfo = gameInfo.playersInfo;
}

void GUIDisplay::displayWinner() const noexcept {
    displayHeads();
    displayPlayers();
    currentMessage = "Player " + std::to_string ( gameInfo.winningPlayerId ) + " wins!";
    acknowledge ( currentMessage );
}

// Process Pagination actions for displaying heads and players
// Returns: true for valid actions; false for invalid actions
bool GUIDisplay::processPaginationAction ( std::string action ) {
    action = to_uppercase ( action );
    if ( action == "NEXT HEADS" || action == "NH" ) {
        displayHeadsNextPage();
    } else if ( action == "PRIOR HEADS" || action == "PH" ) {
        displayHeadsPriorPage();
    } else if ( action == "NEXT PLAYERS" || action == "NP" ) {
        displayPlayersNextPage();
    } else if ( action == "PRIOR PLAYERS" || action == "PP" ) {
        displayPlayersPriorPage();
    } else {
        return false;
    }
    return true;
}

int GUIDisplay::inputNumPlayers ( ) {
    int numOfPlayers = 0;
    std::string question = PROMPT_NUM_OF_PLAYERS;
    while ( numOfPlayers <= 1 ) {
        std::string answer = inputString ( grammarCorrection (question ) );
        try {
            numOfPlayers = std::stoi ( answer );
        } catch (std::invalid_argument &) {
            displayErrorMessage ( ERROR_MSG_INVALID_INPUT );
        }
    }
    clearErrorMessage();
    return numOfPlayers;
}

void GUIDisplay::inputToAcceptATurn ( int playerId, bool isComputerPlayer ) {
    std::string promptStr = grammarCorrection ( PROMPT_START_A_TURN_1 + std::to_string(playerId) + PROMPT_START_A_TURN_2 );
    if ( isComputerPlayer ) {
        displayMessage ( "Computer " + promptStr );
        displayAnswer (  "Computer player has accepted" );
        return;
    }
    while ( true ) {
        std::string answer = inputString ( promptStr );
        if ( processPaginationAction ( answer ) ) {
            continue;
        } else if ( answer == "q" || answer == "Q" ) {
            throw PlayerQuitException{};
        } else {
            break;
        }
    }
    clearErrorMessage();
}

int GUIDisplay::inputHeadId ( int playerId, std::string cardDisplayName, bool previousAnswerHasError, int preAnsweredHeadNo ) {

    int headNo = -1;
    std::string promptStr = grammarCorrection ( PROMPT_CHOOSE_HEAD_1 + std::to_string(playerId) + PROMPT_CHOOSE_HEAD_2 + cardDisplayName + PROMPT_CHOOSE_HEAD_3 );
    if ( preAnsweredHeadNo >= 0 ) {
        displayMessage ( "Computer " + promptStr );
        displayAnswer (  std::to_string(preAnsweredHeadNo) );
        return preAnsweredHeadNo;
    }
    if ( previousAnswerHasError ) {
        displayErrorMessage ( ERROR_MSG_INVALID_INPUT );
    }

    while ( headNo < 0 ) {
        std::string answer = inputString ( promptStr );
        if ( processPaginationAction ( answer ) ) {
            continue;
        } else {
            try {
                headNo = std::stoi ( answer );
            } catch (std::invalid_argument &) {
                displayErrorMessage ( ERROR_MSG_INVALID_INPUT );
            }
        }
    }
    clearErrorMessage();
    return headNo;
}

int GUIDisplay::inputJokerValue ( int preAnsweredJokerValue ) {
    std::string jokerRank;
    std::string prompt = grammarCorrection ( PROMPT_JOKER_VALUE );
    if ( preAnsweredJokerValue > 0 ) {
        displayMessage ( prompt );
        displayAnswer ( convertFaceValueToString ( preAnsweredJokerValue ) );
        return preAnsweredJokerValue;
    }
    while ( true ) {
        jokerRank = inputString ( prompt );
        if ( processPaginationAction ( jokerRank ) ) {
            continue;
        } else {
            int faceValue = convertFaceValueToInt ( jokerRank );
            if ( faceValue <= 0 ) {
                displayErrorMessage ( ERROR_MSG_INVALID_INPUT );
            } else {
                clearErrorMessage();
                return faceValue;
            }
        }
    }
}

// Asks card value in testing mode, if the card value is not Joker, asks for suit.
// Returns a string in format of "S#", S - H/S/D/C/J (J - Joker), # - 1-13
std::string GUIDisplay::promptForCardValue ( std::string reason ) {
    std::string promptStr = grammarCorrection ( PROMPT_FOR_TESTING_1 + reason + PROMPT_FOR_TESTING_2 + PROMPT_CARD_VALUE );
    while ( true ) {
        std::string cardRank = inputString ( promptStr );
        int faceValue = convertFaceValueToInt ( cardRank ); // not process for pagination in testing mode
        if ( faceValue < 0 ) {
            displayErrorMessage ( ERROR_MSG_INVALID_INPUT );
            continue;
        } else if ( faceValue == 0 ) {  // Joker
            clearErrorMessage();
            return "J";
        }
        clearErrorMessage();
        std::string suit = promptForSuit(reason);
        return suit + std::to_string ( faceValue );
    }
}

std::string GUIDisplay::promptForSuit ( std::string reason ) {
    std::string promptStr = grammarCorrection ( PROMPT_FOR_TESTING_1 + reason + PROMPT_FOR_TESTING_2 + PROMPT_SUIT );
    while ( true ) {
        std::string suit = inputString ( promptStr );
        suit = to_uppercase ( suit );
        if ( suit == "H"  || suit == "S" || suit == "D" || suit == "C" ) {
            clearErrorMessage();
            return suit;
        } else {
            displayErrorMessage ( ERROR_MSG_INVALID_INPUT );
        }
    }
}

void GUIDisplay::displayAnswer ( const std::string & answer ) const {
    clearAnswer();
    drawString ( INPUT_POS_X, INPUT_POS_Y, answer );
}

void GUIDisplay::displayHeadsNextPage() {
    int numOfHeadsPerPage = NUM_OF_HEAD_ROWS_ON_SCREEN * NUM_OF_HEAD_COLUMNS_ON_SCREEN;
    int numOfHeads = gameInfo.headsInfo.size();
    if ( firstHeadSeqNoOnScreen + numOfHeadsPerPage <= numOfHeads ) {
        firstHeadSeqNoOnScreen += numOfHeadsPerPage;
        previousGameInfo.headsInfo.clear();
        initHeadsDisplay();
        displayHeads();
    }
}

void GUIDisplay::displayHeadsPriorPage() {
    int numOfHeadsPerPage = NUM_OF_HEAD_ROWS_ON_SCREEN * NUM_OF_HEAD_COLUMNS_ON_SCREEN;
    if ( firstHeadSeqNoOnScreen >= numOfHeadsPerPage ) {
        firstHeadSeqNoOnScreen -= numOfHeadsPerPage;
        previousGameInfo.headsInfo.clear();
        initHeadsDisplay();
        displayHeads();
    }
}

void GUIDisplay::displayPlayersNextPage() {
    int numOfPlayers = gameInfo.playersInfo.size();
    if ( firstPlayerSeqNoOnScreen + NUM_OF_PLAYERS_ON_SCREEN <= numOfPlayers ) {
        firstPlayerSeqNoOnScreen += NUM_OF_PLAYERS_ON_SCREEN;
        previousGameInfo.playersInfo.clear();
        initPlayersDisplay();
        displayPlayers();
    }
}

void GUIDisplay::displayPlayersPriorPage() {
    if ( firstPlayerSeqNoOnScreen >= NUM_OF_PLAYERS_ON_SCREEN ) {
        firstPlayerSeqNoOnScreen -= NUM_OF_PLAYERS_ON_SCREEN;
        previousGameInfo.playersInfo.clear();
        initPlayersDisplay();
        displayPlayers();
    }
}

std::string GUIDisplay::inputString ( const std::string & question ) const {
    if ( question != "" ) {
        displayMessage ( question );
    }
    return trim ( getInput ( INPUT_POS_X, INPUT_POS_Y ) );
}

void GUIDisplay::clearErrorMessage ( ) const {
    currentErrorMessage = "";
    fillRectangle ( ERR_MSG_POS_X, ERR_MSG_POS_Y - 20, ERR_MSG_WIDTH, ERR_MSG_HEIGHT, GameBackgroundColour );
}

void GUIDisplay::displayErrorMessage ( const std::string & message ) const {
    clearErrorMessage();
    currentErrorMessage = message;
    drawString ( ERR_MSG_POS_X, ERR_MSG_POS_Y, message, Red );
}

void GUIDisplay::displayMessage ( const std::string & message ) const {
    clearMessage();
    currentMessage = message;
    drawString ( MESSAGE_POS_X, MESSAGE_POS_Y, message );
}

void GUIDisplay::clearMessage ( ) const {
    currentMessage = "";
    fillRectangle ( MESSAGE_POS_X, MESSAGE_POS_Y - 20, MESSAGE_WIDTH, MESSAGE_HEIGHT, GameBackgroundColour );
}

void GUIDisplay::acknowledge ( const std::string & message ) const {
    inputString ( message + " " + "(Press ENTER to continue)" );
}

void GUIDisplay::clearAnswer ( ) const {
    fillRectangle ( INPUT_POS_X, INPUT_POS_Y - 20, INPUT_WIDTH, INPUT_HEIGHT, GameBackgroundColour );
}

bool GUIDisplay::hasPlayerInfoChanged ( int index ) const {
    int previousPlayersInfoSize = previousGameInfo.playersInfo.size();
    if ( index >= previousPlayersInfoSize ) {
        return true;
    } else {
        return !( previousGameInfo.playersInfo[index].playerId == gameInfo.playersInfo[index].playerId
                && previousGameInfo.playersInfo[index].numOfDrawPile == gameInfo.playersInfo[index].numOfDrawPile
                && previousGameInfo.playersInfo[index].numOfDiscardPile == gameInfo.playersInfo[index].numOfDiscardPile
                && previousGameInfo.playersInfo[index].numOfReservedCards == gameInfo.playersInfo[index].numOfReservedCards
                && previousGameInfo.playersInfo[index].cardInHand == gameInfo.playersInfo[index].cardInHand
                && previousGameInfo.playersInfo[index].numOfRemainingCards == gameInfo.playersInfo[index].numOfRemainingCards
                && previousGameInfo.playersInfo[index].isTurn == gameInfo.playersInfo[index].isTurn
                && previousGameInfo.playersInfo[index].isComputerPlayer == gameInfo.playersInfo[index].isComputerPlayer );
    }
}

bool GUIDisplay::hasHeadInfoChanged ( int index ) const {
    int previousHeadsInfoSize = previousGameInfo.headsInfo.size();
    if ( index >= previousHeadsInfoSize ) {
        // this is a new head (when a cut-off happens)
        return true;
    } else {
        return !( previousGameInfo.headsInfo[index].headId == gameInfo.headsInfo[index].headId
                && previousGameInfo.headsInfo[index].numOfCards == gameInfo.headsInfo[index].numOfCards
                && previousGameInfo.headsInfo[index].topCard == gameInfo.headsInfo[index].topCard );
    }
}

void GUIDisplay::drawImageIfNotExist ( const std::string & imageName, int x, int y ) const {
    // Draw card suit
    std::shared_ptr<PokerCardPixel> cardPixel = pokerCardPixelLibrary.getCardSuitPixel ( imageName );
    int colour = cardPixel->isRedColour() ? Red : Black;
    int width = cardPixel->getWidth();
    int height = cardPixel->getHeight();
    if ( width != 0 ) {
        fillRectangle ( x, y, CARD_WIDTH, CARD_HEIGHT, White );
        int ratio = 2;
        if ( to_uppercase ( imageName ) == to_uppercase ( IMAGE_NAME_CARD_BACK ) ) {
            drawImage ( x, y, cardPixel, 5, Red );
            return;
        } else  if ( to_uppercase ( imageName ) == to_uppercase ( IMAGE_NAME_LOGO ) ) {
            fillRectangle ( x, y, LOGO_WIDTH, LOGO_HEIGHT, GameBackgroundColour );
            drawImage ( x, y, cardPixel, 10, Yellow );
            return;
        }
        if ( to_uppercase ( imageName ) == to_uppercase ( IMAGE_NAME_JOKER ) ) {
            ratio = 1;
        }
        // top-left cornor
        int imageX = x + 5;
        int imageY = y + 5;
        drawImage ( imageX, imageY, cardPixel, ratio, colour );

        // bottom-right cornor
        imageX = x + CARD_WIDTH - width * ratio - 5;
        imageY = y + CARD_HEIGHT - height * ratio - 5;
        drawImage ( imageX, imageY, cardPixel, ratio, colour );
    }

    // Draw card face value
    cardPixel = pokerCardPixelLibrary.getCardValuePixel ( imageName );
    width = cardPixel->getWidth();
    height = cardPixel->getHeight();
    if ( width != 0 ) {
        int ratio = 3;
        if ( to_uppercase ( imageName ) == to_uppercase ( IMAGE_NAME_JOKER ) ) {
            // draw Joker as value
            ratio = 2;
        }
        int imageX = x + CARD_WIDTH / 2 - width * ratio / 2;
        int imageY = y + CARD_HEIGHT / 2 - height * ratio / 2;
        drawImage ( imageX, imageY, cardPixel, ratio, colour );
    }
}

void GUIDisplay::drawImage ( int x, int y, std::shared_ptr<PokerCardPixel> cardPixel, int ratio, int colour ) const {
    int width = cardPixel->getWidth();
    int height = cardPixel->getHeight();
    for ( int i = 0; i < height; i ++ ) {
        for ( int j = 0; j < width; j ++ ) {
            std::string pixel = cardPixel->getPixels().substr ( i * width + j, 1 );
            if ( pixel == "1" ) {
                fillRectangle ( x + j * ratio, y + i * ratio, ratio, ratio, colour );
            }
        }
    }
}

void GUIDisplay::drawImage ( int x, int y, std::string imageKey, int ratio, int colour ) const {
    std::shared_ptr<PokerCardPixel> cardPixel = pokerCardPixelLibrary.getCardSuitPixel ( imageKey );
    if ( cardPixel->getWidth() > 0 ) {
        drawImage ( x, y, cardPixel, ratio, colour );
    }
}
