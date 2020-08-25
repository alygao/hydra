#include <algorithm>
#include <memory>

#include "beginnerComputerPlayingStrategy.h"
#include "game.h"
#include "card.h"
#include "head.h"
#include "player.h"

// ------------------- BeginnerComputerPlayingStrategy implementation -----------------------

BeginnerComputerPlayingStrategy::~BeginnerComputerPlayingStrategy() { }

// Simple AI strategy, returns the no. of head that has the smallest top card face value but larger than the playing card's face value.
// If no head found, it chooses the first head with the same top card face value as the player card.
// If the playingCard is Ace, place it on the head with the largest top card.
// This strategy never uses reserve
int BeginnerComputerPlayingStrategy::chooseHeadNo ( const std::shared_ptr<Card> playingCard, const Game & game, const std::shared_ptr<Player> currentPlayer ) {
  int headNo = 0;
  int smallestFaceValueButLargerThanPlayerCard = 15;
  int headNoForLargestTopCard = 0;
  int largestFaceValue = 0;
  int headNoForSameTopCardFaceValue = 0;

  for ( auto head : game.getHeads() ) {

    // // find the head with the largest top card
    if ( largestFaceValue < head.second->getTopCard()->getFaceValue() ) {
      largestFaceValue = playingCard->getFaceValue();
      headNoForLargestTopCard = head.first;
    }

    // find the head with the smallest top card but still larger than the playingCard
    int headTopCardfaceValue = head.second->getTopCard()->getFaceValue();
    if ( headTopCardfaceValue == 1 ) {
        headTopCardfaceValue = 14;
    }
    if ( smallestFaceValueButLargerThanPlayerCard > headTopCardfaceValue && headTopCardfaceValue > playingCard->getFaceValue() ) {
      smallestFaceValueButLargerThanPlayerCard = headTopCardfaceValue;
      headNo = head.first;
    }

    // find the first head with the same top card face value as the playingCard
    if ( headNoForSameTopCardFaceValue == 0 && head.second->getTopCard()->getFaceValue() == playingCard->getFaceValue() ) {
      headNoForSameTopCardFaceValue = head.first;
    }
  }
  if ( playingCard->getFaceValue() == 1 ) { // if playingCard is Ace
      headNo = headNoForLargestTopCard;
  } else if ( headNo == 0 ) {
    if ( headNoForSameTopCardFaceValue != 0 ) {
      headNo = headNoForSameTopCardFaceValue;
    } else { // all heads' topCard's faceValue < playingCard.faceValue, take the first head.
      headNo = game.getHeads().begin()->first;
    }
  }
  // For safety purpose, if the calculated headNo is invalid, use the first valide headNo.
  if ( !validateHeadNo ( headNo, playingCard->getFaceValue(), game ) ) {
    headNo = getFirstValidHeadNo ( playingCard->getFaceValue(), game );
  }
  return headNo;
}

// Simple strategy, always returns Ace, i.e., 1
int BeginnerComputerPlayingStrategy::announceJokerValue ( const Game & game, const std::shared_ptr<Player> currentPlayer  ) {
  return 1;
}

