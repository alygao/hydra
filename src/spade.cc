#include "spade.h"

Spade::Spade(std::shared_ptr<Card> component) : Suit{component} {};

std::string Spade::getDisplayName() const noexcept {
    std::string name = component->getDisplayName() + "S";
    return name;
}
