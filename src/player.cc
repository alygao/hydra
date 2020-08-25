#include "player.h"
#include "card.h"
#include "util.h"

Player::Player ( int id, std::vector<std::shared_ptr<Card>> drawPile ) : playerId { id }, drawPile { drawPile } { 
    reserveCard = nullptr;
    inTurn = false;
    status = PlayerStatus::HumanPlayer;
}
bool Player::hasWon() const{
    return drawPile.empty() && discardPile.empty() && (reserveCard == nullptr);
}
std::shared_ptr<Card> Player::takeCard() {
    std::shared_ptr<Card> card = nullptr;
    if (drawPile.size() > 0) {
        card = drawPile.front();
        drawPile.erase(drawPile.begin());
    } else if (discardPile.size() > 0) {
        shuffle(discardPile);
        drawPile.insert(drawPile.end(), discardPile.begin(), discardPile.end());
        discardPile.clear();
        card = drawPile.front();
        drawPile.erase(drawPile.begin());
    }
    cardInHand = card;
    return card;
}

int Player::getDrawPileSize() const noexcept{
    return drawPile.size();
}

int Player::getDiscardPileSize() const noexcept{
    return discardPile.size();
}

void Player::swapWithReserveCard(std::shared_ptr<Card> & card) {
    std::swap(reserveCard, card);
    cardInHand = card;
}
void Player::placeCardBackToDrawPile(std::shared_ptr<Card> card) {
    drawPile.insert(drawPile.begin(), card);
}

void Player::addToDiscardPile(std::shared_ptr<Card> card) {
    discardPile.emplace_back(card);
}

void Player::addToDiscardPile ( std::vector<std::shared_ptr<Card>> cards ) {
    std::copy ( cards.begin(), cards.end(), std::back_inserter ( discardPile ) );
}

void Player::endTurn() {
    inTurn = false;
    numOfRemainingCards = 0;
    cardInHand = nullptr;
    if (reserveCard != nullptr) {
        addToDiscardPile(reserveCard);
        reserveCard = nullptr;
    }
}

void Player::setInTurn(bool inTurn) {
    this->inTurn = inTurn;
}
void Player::setNumRemainingCards(int numCards) {
    this->numOfRemainingCards = numCards;
}

void Player::decreaseNumRemainingCardsByOne() {
   numOfRemainingCards -= 1; 
}

void Player::increaseNumRemainingCardsByOne() {
   numOfRemainingCards += 1; 
}

std::shared_ptr<Card> Player::getCardInHand() const noexcept{
    return cardInHand;
}
bool Player::hasReservedCard() {
    return reserveCard != nullptr;
}

int Player::getNumRemainingCards() const noexcept{
    return numOfRemainingCards;
}
bool Player::getInTurn() const noexcept{
    return inTurn;
}

int Player::getPlayerId() const noexcept{
    return playerId;
}

std::shared_ptr<Card> Player::getReserveCard() const noexcept{
    return reserveCard;
}

void Player::setReserveCard(std::shared_ptr<Card> card) {
    reserveCard = card;
}
void Player::setCardInHand(std::shared_ptr<Card> card) {
    cardInHand = card;
}

PlayerStatus Player::getStatus() const noexcept{
    return status;
}

void Player::setStatus ( PlayerStatus status ) noexcept {
    this->status = status;
}

