#include "jokerCard.h"
#include "cardProcessor.h"

JokerCard::JokerCard () : Card{0} {};

std::string JokerCard::getDisplayName() const noexcept {
    if (faceValue == 0) {
        return "Joker";     
    } else {
        return Card::getDisplayName() + "J";
    }
};

 void JokerCard::setFaceValue(int value) {
     faceValue = value;
 }

int JokerCard::accept(CardProcessor &c) {
    return c.getCardFaceValue(*this);
}
