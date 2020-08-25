#include <algorithm>
#include "pokerCardPixelLibrary.h"
#include "pokerCardPixel.h"
#include "util.h"

PokerCardPixelLibrary::PokerCardPixelLibrary ( ) {

  pixelMap["A"] = std::make_shared<PokerCardPixel> ( 4, 7, A );
  pixelMap["2"] = std::make_shared<PokerCardPixel> ( 4, 7, N2 );
  pixelMap["3"] = std::make_shared<PokerCardPixel> ( 4, 7, N3 );
  pixelMap["4"] = std::make_shared<PokerCardPixel> ( 4, 7, N4 );
  pixelMap["5"] = std::make_shared<PokerCardPixel> ( 4, 7, N5 );
  pixelMap["6"] = std::make_shared<PokerCardPixel> ( 4, 7, N6 );
  pixelMap["7"] = std::make_shared<PokerCardPixel> ( 4, 7, N7 );
  pixelMap["8"] = std::make_shared<PokerCardPixel> ( 4, 7, N8 );
  pixelMap["9"] = std::make_shared<PokerCardPixel> ( 4, 7, N9 );
  pixelMap["10"] = std::make_shared<PokerCardPixel> ( 8, 7, N10 );
  pixelMap["J"] = std::make_shared<PokerCardPixel> ( 4, 7, J );
  pixelMap["Q"] = std::make_shared<PokerCardPixel> ( 5, 7, Q );
  pixelMap["K"] = std::make_shared<PokerCardPixel> ( 4, 7, K );
  pixelMap["JOKER"] = std::make_shared<PokerCardPixel> ( 21, 7, JOKER );
  pixelMap["H"] = std::make_shared<PokerCardPixel> ( 7, 7, Heart, true );
  pixelMap["S"] = std::make_shared<PokerCardPixel> ( 7, 7, Spade );
  pixelMap["C"] = std::make_shared<PokerCardPixel> ( 7, 7, Club );
  pixelMap["D"] = std::make_shared<PokerCardPixel> ( 7, 7, Diamond, true );
  pixelMap["CARD_BACK"] = std::make_shared<PokerCardPixel> ( 12, 18, CardBack );
  pixelMap["HYDRA_LOGO"] = std::make_shared<PokerCardPixel> ( 67, 10, Logo );

}

std::shared_ptr<PokerCardPixel> PokerCardPixelLibrary::getPixel ( const std::string & pixelName ) const noexcept {
    std::string pixelNameUpperCase = to_uppercase ( pixelName );
    auto p = pixelMap.find ( pixelNameUpperCase );
    if ( p == pixelMap.end() ) {
        return std::make_shared<PokerCardPixel> ( 0, 0, "" );
    } else {
        return p->second;
    }
}

std::shared_ptr<PokerCardPixel> PokerCardPixelLibrary::getCardValuePixel ( const std::string & pixelName ) const noexcept {
    std::string pixelNameUpperCase = to_uppercase ( pixelName );
    if ( pixelNameUpperCase == "JOKER" || pixelNameUpperCase == "CARD_BACK" || pixelNameUpperCase == "HYDRA_LOGO" ) {
        return getPixel ( pixelNameUpperCase );
    }
    if ( pixelNameUpperCase.size() > 0 ) {
        return getPixel ( pixelNameUpperCase.substr ( 0, pixelNameUpperCase.size() - 1 ) );
    } else {
        return std::make_shared<PokerCardPixel> ( 0, 0, "" );
    }
}

std::shared_ptr<PokerCardPixel> PokerCardPixelLibrary::getCardSuitPixel ( const std::string & pixelName ) const noexcept {
    std::string pixelNameUpperCase = to_uppercase ( pixelName );
    if ( pixelNameUpperCase == "JOKER" || pixelNameUpperCase == "CARD_BACK" || pixelNameUpperCase == "HYDRA_LOGO" ) {
        return getPixel ( pixelNameUpperCase );
    }
    if ( pixelNameUpperCase.size() > 0 ) {
        return getPixel ( pixelNameUpperCase.substr ( pixelNameUpperCase.size() - 1 ) );
    } else {
        return std::make_shared<PokerCardPixel> ( 0, 0, "" );
    }
}
