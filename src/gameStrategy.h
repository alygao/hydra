#ifndef __GAME_STRATEGY_H__
#define __GAME_STRATEGY_H__

#include <iostream>
#include <memory>

class Game;
class Card;
class Player;

class GameStrategy {
  private:
    bool canCardBePlacedOnAnotherHead(int headNo, int cardFaceValue, const Game & game);
  protected:
    int getFirstValidHeadNo(int cardFaceValue, const Game & game);
    bool validateHeadNo(int headNo, int cardFaceValue, const Game & game);

  public:
    virtual int chooseHeadNo ( const std::shared_ptr<Card> playingCard, const Game & game, const std::shared_ptr<Player> currentPlayer  ) = 0; // Returns a headNo based on the game's current state. Returns -1 if no strategy is implemented.
    virtual int announceJokerValue ( const Game & game, const std::shared_ptr<Player> currentPlayer  ) = 0;   // Returns the announced value for Joker. Returns 0 if no strategy is implemented.
    virtual ~GameStrategy();
};

#endif
