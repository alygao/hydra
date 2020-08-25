#ifndef __GAME_CREATOR_H__
#define __GAME_CREATOR_H__

#include <iostream>
#include <vector>
#include <memory>

class Game;
struct GameOptions;

class GameCreator {    
  public:
    GameCreator ( );
    virtual std::shared_ptr<Game> newGame ( GameOptions gameOptions ) = 0;
    virtual ~GameCreator();
};

#endif
