#include <map>
#include <vector>
#include <unistd.h>
#include <algorithm>
#include "game.h"
#include "gameInfo.h"
#include "card.h"
#include "head.h"
#include "player.h"
#include "gameDisplay.h"
#include "util.h"
#include "jokerCard.h"
#include "spade.h"
#include "heart.h"
#include "club.h"
#include "diamond.h"
#include "cardValueProcessor.h"
#include "gameStrategy.h"


static const int NUM_OF_CARDS_PER_DECK = 54;
static const int NUM_OF_CARDS_PER_SUIT = 13;

static const std::string REASON_MSG_FOR_TESTING_INPUT_CREATE_FIRST_HEAD = "creating the first head"; 
static const std::string REASON_MSG_FOR_TESTING_INPUT_PLAYER_NEXT_INHAND_CARD = "player's next in-hand card";  
static const std::string REASON_MSG_FOR_TESTING_INPUT_CREATE_FIRST_HEAD_PER_CUTOFF = "creating the first head as a result of cut-off head";  
static const std::string REASON_MSG_FOR_TESTING_INPUT_CREATE_SECOND_HEAD_PER_CUTOFF = "creating the second head as a result of cut-off head"; 

Game::Game(std::shared_ptr<GameDisplay> display, GameOptions gameOptions, std::shared_ptr<GameStrategy> gameStrategy) 
            : display{display}, gameOptions{gameOptions}, gameStrategy{gameStrategy} {
    winner = nullptr;
    attach(display);
}

Game::~Game() {}

void Game::startGame() {
    CardValueProcessor cvp;
    try {
        initGame();
        initialMove();
        while (winner == nullptr) {
            endCurrentPlayerTurn();
            notifyObservers();
            display->displayHeadsAndPlayers();
            moveToNextPlayer();

            while ( currentPlayer->getStatus() == PlayerStatus::Quit ) moveToNextPlayer();
            try {
                display->inputToAcceptATurn( currentPlayer->getPlayerId(), currentPlayer->getStatus() == PlayerStatus::ComputerPlayer );
                if ( currentPlayer->getStatus() == PlayerStatus::ComputerPlayer ) { usleep ( gameOptions.computerPlayerActionElapseTime * 1000 ); }
            } catch ( PlayerQuitException ) {
                if ( gameStrategy == nullptr ) { 
                    // no game strategy (i.e., no in enhancement mode, no computer player ), so just remove the human player, 
                    // if there is only one human player, then quit the game.
                    int numOfActivePlayers = std::count_if ( players.begin(), players.end(), [](auto player) { return player->getStatus() != PlayerStatus::Quit; } );
                    if ( numOfActivePlayers > 2 ) {
                        display->acknowledge ( "Player " + std::to_string ( currentPlayer->getPlayerId() ) + " quits, moving to the next player ..." );
                        currentPlayer->setStatus ( PlayerStatus::Quit );
                        continue;
                    } else {
                        display->acknowledge ( "Player " + std::to_string ( currentPlayer->getPlayerId() ) + " quits, only one player left, the game terminates.");
                        return;
                    }
                } else {
                    display->acknowledge ( "Player " + std::to_string ( currentPlayer->getPlayerId() ) + " quits, replaced by computer player ..." );
                    currentPlayer->setStatus ( PlayerStatus::ComputerPlayer );
                }
            }

            initTurnForPlayer();
            bool isTopCardFromReserve = false;
            while (currentPlayer->getNumRemainingCards() > 0) {
                std::shared_ptr<Card> cardInHand = takeACardFromCurrentPlayer(REASON_CODE_FOR_TESTING_INPUT_PLAYER_NEXT_INHAND_CARD, isTopCardFromReserve);
                isTopCardFromReserve = false;
                if (cardInHand == nullptr) {
                    currentPlayer->setNumRemainingCards(0);
                    continue;
                }
                notifyObservers();
                display->displayHeadsAndPlayers();

                int headNo = -1;
                if ( currentPlayer->getStatus() == PlayerStatus::ComputerPlayer ) { // no need to check if gameStrategy != nullptr, as it will be checked when a computer player is switched to.
                    // computer player
                    headNo = gameStrategy->chooseHeadNo ( cardInHand, *this, currentPlayer );
                    headNo = display->inputHeadId ( currentPlayer->getPlayerId(), cardInHand->getDisplayName(), false, headNo );
                    usleep ( gameOptions.computerPlayerActionElapseTime * 1000 );
                    int cardFaceValue = 0;
                    try {
                        cardFaceValue = cardInHand->accept(cvp); 
                    } catch (JokerValueRequiredException &) {
                        cardFaceValue = gameStrategy->announceJokerValue ( *this, currentPlayer );
                        cardFaceValue = display->inputJokerValue(cardFaceValue);
                        usleep ( gameOptions.computerPlayerActionElapseTime * 1000 );
                    }
                    cardInHand->setFaceValue ( cardFaceValue );
                } else {
                    // human player
                    headNo = display->inputHeadId(currentPlayer->getPlayerId(), cardInHand->getDisplayName());
                    while (true) {
                        while (!validateHeadNo(headNo, cardInHand->getFaceValue())) {
                            headNo = display->inputHeadId(currentPlayer->getPlayerId(), cardInHand->getDisplayName(), true);
                        }
                        if (headNo == 0) { break; }
                        int cardFaceValue = 0;
                        try {
                            cardFaceValue = cardInHand->accept(cvp);
                            break;
                        } catch (JokerValueRequiredException &) {
                            cardFaceValue = display->inputJokerValue();
                            while (cardFaceValue < 1 || cardFaceValue > 13) {
                                cardFaceValue = display->inputJokerValue();
                            }
                            if (validateHeadNo(headNo, cardFaceValue)) {
                                cardInHand->setFaceValue(cardFaceValue);
                                break;
                            }
                        }
                        headNo = display->inputHeadId(currentPlayer->getPlayerId(), cardInHand->getDisplayName(), true);
                    }
                }
                // Reserve 
                if (headNo == 0) {
                    placeCardInReserve(currentPlayer, cardInHand);
                    if (cardInHand != nullptr) { // player already has a reserve card
                        isTopCardFromReserve = true;
                        returnCardBackToDrawPile (currentPlayer, cardInHand);
                    }
                    continue;
                }

                int result = placeCardOnHead(cardInHand, headNo);
                if (result > 0) {
                    cutoffHead();
                    currentPlayer->setNumRemainingCards(0);
                    if (currentPlayer->hasWon()) {
                        winner = currentPlayer;
                        currentPlayer->endTurn();
                    }
                    break;
                } else if (result == 0) { // turn is terminated immediately
                    currentPlayer->setNumRemainingCards(0); 
                } // else continue turn
                if (currentPlayer->hasWon()) {
                    winner = currentPlayer;
                    currentPlayer->endTurn();
                    break;
                }
            }
            if (currentPlayer->hasWon()) {
                winner = currentPlayer;
                currentPlayer->endTurn();
                break;
            }
        }
        notifyObservers();
        display->displayHeadsAndPlayers();
    } catch ( PlayerTerminateException ) {
        display->acknowledge ( "Player has terminated the game." );
        return;
    }
}

void Game::initGame() {
    int numOfPlayers = display->inputNumPlayers();
    cards = generateCards ( numOfPlayers );
    int numOfCardPerPlayer = gameOptions.testingMode ? gameOptions.numOfCardPerPlayer : NUM_OF_CARDS_PER_DECK;
    shuffle ( cards );
    for ( int i = 0; i < numOfPlayers; i ++ ) {
        std::vector<std::shared_ptr<Card>> cardForPlayer;
        std::copy ( cards.begin() + i * numOfCardPerPlayer, cards.begin() + (i + 1) * numOfCardPerPlayer, std::back_inserter(cardForPlayer) );
        std::shared_ptr<Player> player = std::make_shared<Player> ( i + 1, cardForPlayer );
        players.emplace_back ( player );
    }
}

GameInfo Game::getGameInfo() const{
    std::vector<HeadInfo> headsInfo;
    std::vector<PlayerInfo> playersInfo;
    int minHeadId = heads.begin()->first;
    for (unsigned int i = 0; i < heads.size(); i++) {
        int headIndex = minHeadId + i;
        std::shared_ptr<Head> currHead = heads.at(headIndex);
        headsInfo.emplace_back(HeadInfo{headIndex, currHead->getSize(), currHead->getTopCard()->getDisplayName()});
    }

    for (auto p : players) {
        if ( p->getStatus() == PlayerStatus::Quit ) {
            continue;
        }
        if (p->getInTurn()) {
            playersInfo.emplace_back(PlayerInfo{p->getPlayerId(), p->getDrawPileSize(), p->getDiscardPileSize(), p->hasReservedCard(), 
                                                p->getCardInHand()->getDisplayName(), p->getNumRemainingCards(), p->getInTurn(), p->getStatus() == PlayerStatus::ComputerPlayer}); // TODO: set computer player
        } else {
            playersInfo.emplace_back(PlayerInfo{p->getPlayerId(), p->getDrawPileSize(), p->getDiscardPileSize(), 0, "", 0, p->getInTurn(), p->getStatus() == PlayerStatus::ComputerPlayer}); // TODO : delete this 
        }
    }

    return GameInfo{headsInfo, playersInfo,winner==nullptr ? 0 : winner->getPlayerId()};
}

std::vector<std::shared_ptr<Card>> Game::generateCards ( int numOfPlayers ) {
    std::vector<std::shared_ptr<Card>> cards;
    for ( int i = 0; i < numOfPlayers; i ++ ) {
        for ( int j = 0; j < NUM_OF_CARDS_PER_SUIT; j ++ ) {
            cards.emplace_back ( std::make_shared<Heart> ( std::make_shared<Card> ( j + 1 ) ) );
            cards.emplace_back ( std::make_shared<Spade> ( std::make_shared<Card> ( j + 1 ) ) );
            cards.emplace_back ( std::make_shared<Club> ( std::make_shared<Card> ( j + 1 ) ) );
            cards.emplace_back ( std::make_shared<Diamond> ( std::make_shared<Card> ( j + 1 ) ) );
        }
        cards.emplace_back ( std::make_shared<JokerCard> ( ) );
        cards.emplace_back ( std::make_shared<JokerCard> ( ) );
    }
    return cards;
}

bool Game::validateHeadNo(int headNo, int cardFaceValue) {
    int minHeadId = heads.begin()->first;
    int maxHeadId = heads.rbegin()->first;

    // Reserve Card
    if (headNo == 0) {
        if (heads.size() == 1) {
            return false;
        } 
        return true; 
    }

    if (headNo >= minHeadId && headNo <= maxHeadId) {
        if (heads[headNo]->getTopCard()->getFaceValue() == 1) { return true; } // Head's top card is ace card
        if (cardFaceValue <= heads[headNo]->getTopCard()->getFaceValue()) {
            return true;
        } else {
            if (!canCardBePlacedOnAnotherHead(headNo, cardFaceValue) && headNo == minHeadId) {
                return true;
            }
        }
    }
    return false;
}

bool Game::canCardBePlacedOnAnotherHead(int headNo, int cardFaceValue) {
    int minHeadId = heads.begin()->first;
    int headIndex = minHeadId + 1; // skip first head
    for (unsigned int i = 0; i < heads.size() - 1; i++) {
        if ((cardFaceValue <= heads[headIndex]->getTopCard()->getFaceValue()) || 
            (heads[headIndex]->getTopCard()->getFaceValue() == 1)) {
            return true;
        }
        headIndex++;
    }
    return false;
}

std::shared_ptr<Card> Game::inputCardValueForTesting ( const std::shared_ptr<Card> card, int reasonCode ) {
    std::shared_ptr<Card> newCard;
    std::string reason = "";
    if ( reasonCode == REASON_CODE_FOR_TESTING_INPUT_CREATE_FIRST_HEAD ) {
        reason = REASON_MSG_FOR_TESTING_INPUT_CREATE_FIRST_HEAD;
    } else if ( reasonCode == REASON_CODE_FOR_TESTING_INPUT_PLAYER_NEXT_INHAND_CARD ) {
        reason = REASON_MSG_FOR_TESTING_INPUT_PLAYER_NEXT_INHAND_CARD;
    } else if ( reasonCode == REASON_CODE_FOR_TESTING_INPUT_CREATE_FIRST_HEAD_PER_CUTOFF ) {
        reason = REASON_MSG_FOR_TESTING_INPUT_CREATE_FIRST_HEAD_PER_CUTOFF;
    } else {
        reason = REASON_MSG_FOR_TESTING_INPUT_CREATE_SECOND_HEAD_PER_CUTOFF;
    }

    std::string cardValue = display->promptForCardValue(reason);
    if ( cardValue == "J" ) {
        return std::make_shared<JokerCard>();
    } else {
        std::string suit = cardValue.substr ( 0, 1 );
        int faceValue = std::stoi ( cardValue.substr ( 1 ) );
        std::shared_ptr<Card> baseCard = std::make_shared<Card> ( faceValue );
        if ( suit == "H" ) {
        return std::make_shared<Heart> ( baseCard );
        } else if ( suit == "S" ) {
        return std::make_shared<Spade> ( baseCard );
        } else if ( suit == "D" ) {
        return std::make_shared<Diamond> ( baseCard );
        } else { //  suit == "C"
        return std::make_shared<Club> ( baseCard );
        }
    }
    return nullptr;
}

std::map<int, std::shared_ptr<Head>> Game::getHeads ( ) const noexcept {
  return heads;
}

