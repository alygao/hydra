#ifndef __POKER_CARD_PIXEL_H__
#define __POKER_CARD_PIXEL_H__

#include <map>

class PokerCardPixel {
    int width;
    int height;
    std::string pixels;
    bool redColour;

  public:
    PokerCardPixel ( int width, int height, std::string pixels, bool redColour = false );

    int getWidth() const noexcept;
    int getHeight() const noexcept;
    std::string getPixels() const noexcept;
    bool isRedColour() const noexcept;
};

#endif
