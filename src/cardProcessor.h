#ifndef __CARDPROCESSOR_H__
#define __CARDPROCESSOR_H__
#include <iostream>

class Card;
class JokerCard;

class CardProcessor {
  public:
    virtual ~CardProcessor();
    virtual int getCardFaceValue(Card card) = 0;
    virtual int getCardFaceValue(JokerCard jokerCard) = 0;
};

#endif

