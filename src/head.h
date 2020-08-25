#ifndef __HEAD_H__
#define __HEAD_H__
#include <iostream>
#include <vector>
#include <memory>

class Card;

class Head {
    int headNum;
    std::vector<std::shared_ptr<Card>> cards;
  public:
    Head(int headNum);
    void placeCard(std::shared_ptr<Card> card);
    std::shared_ptr<Card> getTopCard() const;
    int getSize() const;
    std::vector<std::shared_ptr<Card>> getCards() const;
    int getHeadNum();

};

#endif
