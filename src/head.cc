#include "head.h"
#include "card.h"

Head::Head(int headNum) : headNum{headNum} {}

void Head::placeCard(std::shared_ptr<Card> card) {
    if (getTopCard() != nullptr) {
        getTopCard()->setFaceValue(0);
    }
    cards.emplace_back(card);
}

std::shared_ptr<Card> Head::getTopCard() const {
    if (cards.size() > 0) {
        return cards.back();
    } else {
        return nullptr;
    }
}

int Head::getSize() const {
    return cards.size();
}

std::vector<std::shared_ptr<Card>> Head::getCards() const {
    return cards;
}

int Head::getHeadNum() {
    return headNum;
}
