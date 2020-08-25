#include "club.h"

Club::Club(std::shared_ptr<Card> component) : Suit{component} {};

std::string Club::getDisplayName() const noexcept {
    std::string name = component->getDisplayName() + "C";
    return name;
}
