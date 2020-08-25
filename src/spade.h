#ifndef __SPADE_H__
#define __SPADE_H__
#include "suit.h"

class Spade : public Suit {
  public:
    Spade(std::shared_ptr<Card> component);
    virtual std::string getDisplayName() const noexcept override;
};

#endif
