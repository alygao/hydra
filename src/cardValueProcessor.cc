#include "cardValueProcessor.h"
#include "card.h"
#include "jokerCard.h"

int CardValueProcessor::getCardFaceValue(Card card) {
    return card.getFaceValue();
}
int CardValueProcessor::getCardFaceValue(JokerCard jokerCard) {
    if (jokerCard.getFaceValue() == 0) {
        throw JokerValueRequiredException {};
    } else {
        return jokerCard.getFaceValue();
    }
}
