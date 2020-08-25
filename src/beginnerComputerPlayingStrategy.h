#ifndef __BEGINNER_COMPUTER_PLAYING_STRATEGY_H__
#define __BEGINNER_COMPUTER_PLAYING_STRATEGY_H__

#include "gameStrategy.h"

class BeginnerComputerPlayingStrategy : public GameStrategy {
  public:
    virtual int chooseHeadNo ( const std::shared_ptr<Card> playingCard, const Game & game, const std::shared_ptr<Player> currentPlayer  ) override; // Returns a headNo based on the game's current state. Returns -1 if no strategy is implemented.
    virtual int announceJokerValue ( const Game & game, const std::shared_ptr<Player> currentPlayer ) override; // Returns the announced value for Joker. Returns 0 if no strategy is implemented.
    virtual ~BeginnerComputerPlayingStrategy() override;
};

#endif
