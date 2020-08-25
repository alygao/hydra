#ifndef __HYDRAGAME_H__
#define __HYDRAGAME_H__
#include <iostream>
#include "game.h"

class HydraGame : public Game {
    virtual void initialMove() override;
    virtual void moveToNextPlayer() override;
    virtual void initTurnForPlayer() override;
    virtual std::shared_ptr<Card> takeACardFromCurrentPlayer(int reasonCode, bool isTopCardFromReserve) override;
    virtual int placeCardOnHead (std::shared_ptr<Card> card, int headNo) override;
    virtual void placeCardInReserve(std::shared_ptr<Player> player, std::shared_ptr<Card> & card) override;
    virtual void returnCardBackToDrawPile(std::shared_ptr<Player> player, std::shared_ptr<Card> card) override;
    virtual void cutoffHead() override;
    virtual void endCurrentPlayerTurn() override;
    void createHead(int reasonCode);
  public:
    HydraGame(std::shared_ptr<GameDisplay> display, GameOptions gameOptions, std::shared_ptr<GameStrategy> gameStrategy = nullptr);
};

#endif
