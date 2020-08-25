#ifndef __GAME_OPTIONS_H__
#define __GAME_OPTIONS_H__

enum class GameDisplayType { Text, GUI };

struct GameOptions {
    bool testingMode = false;
    int numOfCardPerPlayer = 54;
    GameDisplayType displayType = GameDisplayType::Text;
    int computerPlayerLevel = 0;
    int computerPlayerActionElapseTime = 1000;
};

#endif
