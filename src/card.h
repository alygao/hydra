#ifndef __CARD_H__
#define __CARD_H__
#include <iostream>

class CardProcessor;

class Card {
  protected:
    int faceValue;
  public:
    Card(int faceValue);
    virtual ~Card();
    virtual std::string getDisplayName() const noexcept;
    int getFaceValue() const noexcept;
    virtual void setFaceValue(int value); 
    virtual int accept(CardProcessor &c);

};

#endif
