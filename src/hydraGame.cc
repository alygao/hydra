#include "hydraGame.h"
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

HydraGame::HydraGame(std::shared_ptr<GameDisplay> display, GameOptions gameOptions, std::shared_ptr<GameStrategy> gameStrategy) 
                        : Game{display, gameOptions, gameStrategy} {
}

void HydraGame::createHead(int reasonCode) {
    int indexForNewHead = 0;
    if (heads.empty()) {
        indexForNewHead = (reasonCode == REASON_CODE_FOR_TESTING_INPUT_CREATE_FIRST_HEAD ? 1 : 2);
        heads[indexForNewHead] = (std::make_shared<Head>(indexForNewHead));
    } else {
        indexForNewHead = heads.rbegin()->first + 1;
        heads[indexForNewHead] = (std::make_shared<Head>(indexForNewHead));
    }
    std::shared_ptr<Card> card = takeACardFromCurrentPlayer(reasonCode, false);
    if (card->getFaceValue() == 0) {
        card->setFaceValue(2); // Joker card
    }
    heads[indexForNewHead]->placeCard(card);
}

void HydraGame::initialMove() {
    currentPlayer = players[0];
    createHead(REASON_CODE_FOR_TESTING_INPUT_CREATE_FIRST_HEAD);
}

void HydraGame::moveToNextPlayer() {
    unsigned int numPlayers = players.size();
    for (unsigned int i = 0; i < numPlayers; i++) {
        if (players[i] == currentPlayer && i < numPlayers - 1) {
            currentPlayer = players[i+1];
            currentPlayer->setInTurn(true);
            return;
        } else if (players[i] == currentPlayer) {
            currentPlayer = players[0];
            currentPlayer->setInTurn(true);
            return;
        }
    }
}

void HydraGame::initTurnForPlayer() {
    currentPlayer->setInTurn(true);
    currentPlayer->setNumRemainingCards(heads.size());
}

std::shared_ptr<Card> HydraGame::takeACardFromCurrentPlayer(int reasonCode, bool isTopCardFromReserve) {
    std::shared_ptr<Card> card = currentPlayer->takeCard();
    if (card != nullptr && gameOptions.testingMode && !isTopCardFromReserve) {
        card = inputCardValueForTesting( card, reasonCode );
        currentPlayer->setCardInHand(card);
    }
    currentPlayer->decreaseNumRemainingCardsByOne();
    return card;
}

int HydraGame::placeCardOnHead (std::shared_ptr<Card> card, int headNo) {
    if (heads[headNo]->getTopCard()->getFaceValue() == 1 || card->getFaceValue() <= heads[headNo]->getTopCard()->getFaceValue()) {
        if (card->getFaceValue() == heads[headNo]->getTopCard()->getFaceValue()) { // end turn
            heads[headNo]->placeCard(card);
            return 0;
        }
        heads[headNo]->placeCard(card);
        return -1;
    } else {
        heads[headNo]->placeCard(card);
        return 1;
    }
}

void HydraGame::placeCardInReserve(std::shared_ptr<Player> player, std::shared_ptr<Card> & card) {
    player->swapWithReserveCard(card);
}

void HydraGame::returnCardBackToDrawPile(std::shared_ptr<Player> player, std::shared_ptr<Card> card) {
    player->placeCardBackToDrawPile(card);
    player->increaseNumRemainingCardsByOne();
}

void HydraGame::cutoffHead() {
    currentPlayer->setCardInHand(nullptr);
    if (currentPlayer->hasReservedCard()) {
        currentPlayer->addToDiscardPile(currentPlayer->getReserveCard());
        currentPlayer->setReserveCard(nullptr);
    }
    int minHeadId = heads.begin()->first;
    std::shared_ptr<Head> oldestHead = heads[minHeadId];
    currentPlayer->addToDiscardPile ( oldestHead->getCards() );
    heads.erase(heads.begin());
    createHead(REASON_CODE_FOR_TESTING_INPUT_CREATE_FIRST_HEAD_PER_CUTOFF);
    createHead(REASON_CODE_FOR_TESTING_INPUT_CREATE_SECOND_HEAD_PER_CUTOFF);
}

void HydraGame::endCurrentPlayerTurn() {
    currentPlayer->endTurn();
}
