#include <algorithm>
#include <memory>

#include "gameStrategy.h"
#include "game.h"
#include "head.h"
#include "card.h"

// ------------------- GameStrategy implementation -----------------------

GameStrategy::~GameStrategy() { }

int GameStrategy::getFirstValidHeadNo(int cardFaceValue, const Game & game) {
    int minHeadId = game.getHeads().begin()->first;
    int maxHeadId = game.getHeads().rbegin()->first;

    for ( int headNo = minHeadId; headNo <= maxHeadId; headNo ++ ) {
        if ( validateHeadNo ( headNo, cardFaceValue, game ) ) {
            return headNo;
        }
    }
    return 0;   // This should not be reached, as at least one headNo must be valid.
}

bool GameStrategy::validateHeadNo(int headNo, int cardFaceValue, const Game & game) {
    int minHeadId = game.getHeads().begin()->first;
    int maxHeadId = game.getHeads().rbegin()->first;

    // Reserve Card
    if (headNo == 0) {
        if (game.getHeads().size() == 1) {
            return false;
        } 
        return true; 
    }

    if (headNo >= minHeadId && headNo <= maxHeadId) {
        if (game.getHeads()[headNo]->getTopCard()->getFaceValue() == 1) { return true; } // Head's top card is ace card
        if (cardFaceValue <= game.getHeads()[headNo]->getTopCard()->getFaceValue()) {
            return true;
        } else {
            if (!canCardBePlacedOnAnotherHead(headNo, cardFaceValue, game) && headNo == minHeadId) {
                return true;
            }
        }
    }
    return false;
}

bool GameStrategy::canCardBePlacedOnAnotherHead(int headNo, int cardFaceValue, const Game & game) {
    int minHeadId = game.getHeads().begin()->first;
    int headIndex = minHeadId + 1; // skip first head
    for (unsigned int i = 0; i < game.getHeads().size() - 1; i++) {
        if ((cardFaceValue <= game.getHeads()[headIndex]->getTopCard()->getFaceValue()) || 
            (game.getHeads()[headIndex]->getTopCard()->getFaceValue() == 1)) {
            return true;
        }
        headIndex++;
    }
    return false;
}

