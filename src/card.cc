#include "card.h"
#include "cardProcessor.h"

Card::Card(int faceValue) : faceValue{faceValue} {};

Card::~Card() {}

std::string Card::getDisplayName() const noexcept{
    std::string name;
    if (faceValue == 11) {
        name = "J";
    } else if (faceValue == 12) {
        name = "Q";
    } else if (faceValue == 13) {
        name = "K";
    } else if (faceValue == 1) {
        name = "A";
    } else {
        name = std::to_string(faceValue);
    }
    return name;
}

int Card::getFaceValue() const noexcept { return faceValue; }

void Card::setFaceValue(int value) { }

int Card::accept(CardProcessor &c) {
    return c.getCardFaceValue(*this);
}
