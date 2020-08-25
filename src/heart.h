#ifndef __HEART_H__
#define __HEART_H__
#include "suit.h"

class Heart : public Suit {
  public:
    Heart(std::shared_ptr<Card> component);
    virtual std::string getDisplayName() const noexcept override;
};

#endif
