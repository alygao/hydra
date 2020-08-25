#include "heart.h"

Heart::Heart(std::shared_ptr<Card> component) : Suit{component} {};

std::string Heart::getDisplayName() const noexcept {
    std::string name = component->getDisplayName() + "H";
    return name;
}
