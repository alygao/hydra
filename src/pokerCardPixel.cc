#include "pokerCardPixel.h"

PokerCardPixel::PokerCardPixel ( int width, int height, std::string pixels, bool redColour ) 
    : width { width }, height { height }, pixels { pixels }, redColour { redColour } {}

int PokerCardPixel::getWidth() const noexcept {
    return width;
}
int PokerCardPixel::getHeight() const noexcept {
    return height;
}
std::string PokerCardPixel::getPixels() const noexcept {
    return pixels;
}

bool PokerCardPixel::isRedColour() const noexcept {
    return redColour;
}
