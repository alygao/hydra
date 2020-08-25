#include <algorithm>
#include <memory>

#include "advancedComputerPlayingStrategy.h"
#include "game.h"
#include "card.h"
#include "head.h"
#include "player.h"

// ------------------- AdvancedComputerPlayingStrategy implementation -----------------------

AdvancedComputerPlayingStrategy::~AdvancedComputerPlayingStrategy() { }

int AdvancedComputerPlayingStrategy::getAverageHeadsTopValue(const Game & game) {
  int sum = 0;
  int minHeadId = game.getHeads().begin()->first;
  int numHeads = game.getHeads().size();
  for (int i = 0; i < numHeads; i++) {
    sum += game.getHeads()[minHeadId]->getTopCard()->getFaceValue();
    if (game.getHeads()[minHeadId]->getTopCard()->getFaceValue() == 1) {
      sum += 13; // since ace is a valuable card, it should increase the sum more
    }
    minHeadId++;
  }
  int averageTopHeadValue = sum / numHeads;
  return averageTopHeadValue;
}

bool AdvancedComputerPlayingStrategy::isHighCardPresent(const Game & game) {
  int minHeadId = game.getHeads().begin()->first;
  int numHeads = game.getHeads().size();
  for (int i = 0; i < numHeads; i++) {
    if (game.getHeads()[minHeadId]->getTopCard()->getFaceValue() > 10 || game.getHeads()[minHeadId]->getTopCard()->getFaceValue() == 1) {
      return true;
    }
  }
  return false;
}

std::vector<std::shared_ptr<Head>> AdvancedComputerPlayingStrategy::getPossibleHeads(const Game & game, const std::shared_ptr<Card> playingCard) {
  std::vector<std::shared_ptr<Head>> possibleHeads;
  int minHeadId = game.getHeads().begin()->first;
  int numHeads = game.getHeads().size();
  for (int i = 0; i < numHeads; i++) {
    int headTopCardFaceValue = game.getHeads()[minHeadId]->getTopCard()->getFaceValue();
    if (headTopCardFaceValue >= playingCard->getFaceValue() || headTopCardFaceValue == 1) {
      possibleHeads.emplace_back(game.getHeads()[minHeadId]);
    }
    minHeadId++;
  }
  return possibleHeads;
}

//aggressive
std::shared_ptr<Head> AdvancedComputerPlayingStrategy::getPossibleHeadWithLargestTopCardValue (std::vector<std::shared_ptr<Head>> possibleHeads) {
  std::shared_ptr<Head> headWithLargestTopCardValue = possibleHeads[0];
  for (unsigned int i = 1; i < possibleHeads.size(); i++) {
    int currHeadFaceValue = possibleHeads[i]->getTopCard()->getFaceValue();
    if (currHeadFaceValue > headWithLargestTopCardValue->getTopCard()->getFaceValue() || currHeadFaceValue == 1) {
      headWithLargestTopCardValue = possibleHeads[i];
    }
  }
  return headWithLargestTopCardValue;
}

// defensive
std::shared_ptr<Head> AdvancedComputerPlayingStrategy::getPossibleHeadWithSmallestTopCardValue (std::vector<std::shared_ptr<Head>> possibleHeads) {
  std::shared_ptr<Head> headWithSmallestTopCardValue = possibleHeads[0];
  for (unsigned int i = 1; i < possibleHeads.size(); i++) {
    if (possibleHeads[i]->getTopCard()->getFaceValue() < headWithSmallestTopCardValue->getTopCard()->getFaceValue()) {
      headWithSmallestTopCardValue = possibleHeads[i];
    }
  }
  return headWithSmallestTopCardValue;
}

// defensive
std::shared_ptr<Head> AdvancedComputerPlayingStrategy::getPossibleHeadWithSmallestTopCardValueNotEqualToPlayingCard (std::vector<std::shared_ptr<Head>> possibleHeads, const std::shared_ptr<Card> playingCard) {
  std::shared_ptr<Head> headWithSmallestTopCardValue = nullptr;
  int smallestTopCardValue = 13;
  if (playingCard->getFaceValue() == 1) { // if playing card is ace, places on head with largest top value
    return getPossibleHeadWithLargestTopCardValue(possibleHeads);
  }
  for (unsigned int i = 1; i < possibleHeads.size(); i++) {
    int currHeadFaceValue = possibleHeads[i]->getTopCard()->getFaceValue();
    if (playingCard->getFaceValue() + 1 <= currHeadFaceValue && currHeadFaceValue < smallestTopCardValue) {
      headWithSmallestTopCardValue = possibleHeads[i];
      smallestTopCardValue = currHeadFaceValue;
    }
  }
  if (headWithSmallestTopCardValue == nullptr) {
    headWithSmallestTopCardValue = possibleHeads[0];
  }
  return headWithSmallestTopCardValue;
}



// Advanced AI strategy . . .
int AdvancedComputerPlayingStrategy::chooseHeadNo ( const std::shared_ptr<Card> playingCard, const Game & game, const std::shared_ptr<Player> currentPlayer ) {
  int headNo = 0;
  std::vector<std::shared_ptr<Head>> possibleHeads = getPossibleHeads(game, playingCard);
  int averageTopHeadValue = getAverageHeadsTopValue(game);
  bool highCardsPresent = isHighCardPresent(game);
  int numHeads = game.getHeads().size();
  int playersRemainingCards = currentPlayer->getNumRemainingCards();
  if (possibleHeads.empty()) { // cp may choose to either reserve or cut off
    if (!(averageTopHeadValue <= 5 && highCardsPresent && playersRemainingCards <= 2)) {
      if (currentPlayer->getReserveCard() == nullptr) {
        headNo = 0; // reserve
      } else {
        std::vector<std::shared_ptr<Head>> possibleHeadsForReserveCard = getPossibleHeads(game, currentPlayer->getReserveCard());
        if (possibleHeadsForReserveCard.empty()) {
          headNo = game.getHeads().begin()->first; // cutoff, since reserve card can't be played either...
        } else {
          headNo = 0;
        }
      }
    } else {
      headNo = game.getHeads().begin()->first; // cutoff
    }
  } else if (averageTopHeadValue < 4 && highCardsPresent && playersRemainingCards <= (numHeads / 2)) { // lower average card values and few cards remaining
      headNo = getPossibleHeadWithLargestTopCardValue(possibleHeads)->getHeadNum();
  } else if (averageTopHeadValue < 4) { // lower average values and lots of cards remaining (prefer to end turn asap)
      headNo = getPossibleHeadWithSmallestTopCardValue(possibleHeads)->getHeadNum();
  } else if (averageTopHeadValue >= 4 && playersRemainingCards <= (numHeads / 2)) {// high average with few cards
      headNo = getPossibleHeadWithLargestTopCardValue(possibleHeads)->getHeadNum();
  } else {
      headNo = getPossibleHeadWithSmallestTopCardValueNotEqualToPlayingCard(possibleHeads, playingCard)->getHeadNum();
  }
  if ( !validateHeadNo ( headNo, playingCard->getFaceValue(), game ) ) {
    headNo = getFirstValidHeadNo ( playingCard->getFaceValue(), game );
  }
  return headNo;
}

// Advanced strategy, always returns Ace or 2
int AdvancedComputerPlayingStrategy::announceJokerValue ( const Game & game, const std::shared_ptr<Player> currentPlayer ) {
  int playersRemainingCards = currentPlayer->getNumRemainingCards();
  if (playersRemainingCards < 2) {
    return 2;
  }
  return 1;
}
