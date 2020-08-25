#ifndef __SUIT_H__
#define __SUIT_H__
#include "card.h"
#include <memory>

class Suit : public Card {
  protected:
    std::shared_ptr<Card> component;
  public:
    Suit(std::shared_ptr<Card> component);
    virtual std::string getDisplayName() const noexcept = 0;
};

#endif
