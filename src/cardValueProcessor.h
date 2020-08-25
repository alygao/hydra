#ifndef __CARDVALUEPROCESSOR_H__
#define __CARDVALUEPROCESSOR_H__
#include <iostream>
#include "cardProcessor.h"

class Card;
class JokerCard;

class JokerValueRequiredException { };

class CardValueProcessor : public CardProcessor {
  public:
    virtual int getCardFaceValue(Card card) override;
    virtual int getCardFaceValue(JokerCard jokerCard) override;
};

#endif
