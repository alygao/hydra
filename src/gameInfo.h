#ifndef __GAMEINFO_H__
#define __GAMEINFO_H__

#include <iostream>
#include <vector>

class Card;

struct HeadInfo {
    int headId;
    int numOfCards;
    std::string topCard;    // displayName in string
};

struct PlayerInfo {
    int playerId;
    int numOfDrawPile;
    int numOfDiscardPile;
    int numOfReservedCards;
    std::string cardInHand; // displayName in string
    int numOfRemainingCards;
    bool isTurn;
    bool isComputerPlayer;
};

struct GameInfo {
    std::vector<HeadInfo> headsInfo;
    std::vector<PlayerInfo> playersInfo;
    int winningPlayerId = 0;
};

#endif
