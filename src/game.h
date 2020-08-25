#ifndef __GAME_H__
#define __GAME_H__
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include "gameSubject.h"
#include "gameOptions.h"

class Card;
class Player;
class Head;
class GameDisplay;
class GameInfo;
class GameStrategy;

const int REASON_CODE_FOR_TESTING_INPUT_CREATE_FIRST_HEAD = 1; 
const int REASON_CODE_FOR_TESTING_INPUT_PLAYER_NEXT_INHAND_CARD = 2; 
const int REASON_CODE_FOR_TESTING_INPUT_CREATE_FIRST_HEAD_PER_CUTOFF = 3; 
const int REASON_CODE_FOR_TESTING_INPUT_CREATE_SECOND_HEAD_PER_CUTOFF = 4; 

class Game : public GameSubject{
  protected:
    std::vector<std::shared_ptr<Card>> cards;
    std::vector<std::shared_ptr<Player>> players;
    std::map<int, std::shared_ptr<Head>> heads;
    std::shared_ptr<GameDisplay> display;
    std::shared_ptr<Player> currentPlayer;
    std::shared_ptr<Player> winner = nullptr;

    GameOptions gameOptions;
    std::shared_ptr<GameStrategy> gameStrategy;
    
    virtual std::shared_ptr<Card> inputCardValueForTesting ( const std::shared_ptr<Card> card, int reasonCode );

  private:
    virtual void initialMove() = 0;
    virtual void moveToNextPlayer() = 0;
    virtual void initTurnForPlayer() = 0;
    virtual std::shared_ptr<Card> takeACardFromCurrentPlayer(int reasonCode, bool isTopCardFromReserve) = 0;
    virtual int placeCardOnHead (std::shared_ptr<Card> card, int headNo) = 0;
    virtual void placeCardInReserve(std::shared_ptr<Player> player, std::shared_ptr<Card> & card) = 0;
    virtual void returnCardBackToDrawPile(std::shared_ptr<Player> player, std::shared_ptr<Card> card) = 0;
    virtual void cutoffHead() = 0;
    virtual void endCurrentPlayerTurn() = 0;
    std::vector<std::shared_ptr<Card>> generateCards(int numOfPlayers);
    bool validateHeadNo(int headNo, int cardFaceValue);
    bool canCardBePlacedOnAnotherHead(int headNo, int cardFaceValue);

  public:
    Game(std::shared_ptr<GameDisplay> display, GameOptions gameOptions, std::shared_ptr<GameStrategy> gameStrategy = nullptr);
    virtual ~Game();
    void initGame();
    void startGame();
    virtual GameInfo getGameInfo() const override;
    virtual std::map<int, std::shared_ptr<Head>> getHeads ( ) const noexcept;


};

#endif
