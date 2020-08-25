#include "suit.h"

Suit::Suit(std::shared_ptr<Card> component) : Card{component->getFaceValue()}, component{component} {};
