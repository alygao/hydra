#ifndef __JOKERCARD_H__
#define __JOKERCARD_H__
#include "card.h"

class CardProcessor;

class JokerCard : public Card {
  public:
    JokerCard ();
    virtual std::string getDisplayName() const noexcept override;
    virtual void setFaceValue(int value) override;
    int accept(CardProcessor &c);
};

#endif
