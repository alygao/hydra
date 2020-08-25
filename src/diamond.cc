#include "diamond.h"

Diamond::Diamond(std::shared_ptr<Card> component) : Suit{component} {};

std::string Diamond::getDisplayName() const noexcept {
    std::string name = component->getDisplayName() + "D";
    return name;
}
