#ifndef __CLUB_H__
#define __CLUB_H__
#include "suit.h"

class Club : public Suit {
  public:
    Club(std::shared_ptr<Card> component);
    virtual std::string getDisplayName() const noexcept override;
};

#endif
