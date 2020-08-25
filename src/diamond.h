#ifndef __DIAMOND_H__
#define __DIAMOND_H__
#include "suit.h"

class Diamond : public Suit {
  public:
    Diamond(std::shared_ptr<Card> component);
    virtual std::string getDisplayName() const noexcept override;
};

#endif
