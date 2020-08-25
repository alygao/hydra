#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <iostream>
#include <vector>
#include <memory>

class Card;

enum class PlayerStatus { HumanPlayer, ComputerPlayer, Quit };

class Player {
    int playerId;
    std::vector<std::shared_ptr<Card>> drawPile;
    std::vector<std::shared_ptr<Card>> discardPile;
    std::shared_ptr<Card> reserveCard = nullptr;
    std::shared_ptr<Card> cardInHand;
    bool inTurn = false;
    int numOfRemainingCards = 0;
    PlayerStatus status = PlayerStatus::HumanPlayer;

  public:
    Player(int id, std::vector<std::shared_ptr<Card>> drawPile);
    bool hasWon() const;
    std::shared_ptr<Card> takeCard();
    int getDrawPileSize() const noexcept;
    int getDiscardPileSize() const noexcept;
    void swapWithReserveCard(std::shared_ptr<Card> & card);
    void placeCardBackToDrawPile(std::shared_ptr<Card> card);
    void addToDiscardPile(std::shared_ptr<Card> card);
    void addToDiscardPile ( std::vector<std::shared_ptr<Card>> cards );
    void endTurn();
    void setInTurn(bool inTurn);
    void setNumRemainingCards(int numCards);
    void decreaseNumRemainingCardsByOne();
    void increaseNumRemainingCardsByOne();
    std::shared_ptr<Card> getCardInHand() const noexcept;
    bool hasReservedCard();
    int getNumRemainingCards() const noexcept;
    bool getInTurn() const noexcept;
    int getPlayerId() const noexcept;
    std::shared_ptr<Card> getReserveCard() const noexcept;
    void setReserveCard(std::shared_ptr<Card> card);
    void setCardInHand(std::shared_ptr<Card> card);
    PlayerStatus getStatus() const noexcept;
    void setStatus ( PlayerStatus status ) noexcept;
};

#endif
