#ifndef __ADVANCED_COMPUTER_PLAYING_STRATEGY_H__
#define __ADVANCED_COMPUTER_PLAYING_STRATEGY_H__

#include <vector>
#include "gameStrategy.h"

class Head;

class AdvancedComputerPlayingStrategy : public GameStrategy {
  private:
    int getAverageHeadsTopValue(const Game & game);
    bool isHighCardPresent(const Game & game);
    std::vector<std::shared_ptr<Head>> getPossibleHeads(const Game & game, const std::shared_ptr<Card> playingCard);
    std::shared_ptr<Head> getPossibleHeadWithLargestTopCardValue (std::vector<std::shared_ptr<Head>> possibleHeads);
    std::shared_ptr<Head> getPossibleHeadWithSmallestTopCardValue (std::vector<std::shared_ptr<Head>> possibleHeads);
    std::shared_ptr<Head> getPossibleHeadWithSmallestTopCardValueNotEqualToPlayingCard (std::vector<std::shared_ptr<Head>> possibleHeads, const std::shared_ptr<Card> playingCard);

  public:
    virtual int chooseHeadNo ( const std::shared_ptr<Card> playingCard, const Game & game, const std::shared_ptr<Player> currentPlayer  ) override; // Returns a headNo based on the game's current state. Returns -1 if no strategy is implemented.
    virtual int announceJokerValue ( const Game & game, const std::shared_ptr<Player> currentPlayer  ) override; // Returns the announced value for Joker. Returns 0 if no strategy is implemented.
    virtual ~AdvancedComputerPlayingStrategy() override;
};

#endif
