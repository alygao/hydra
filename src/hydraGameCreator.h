#ifndef __HYDRA_GAME_CREATOR_H__
#define __HYDRA_GAME_CREATOR_H__

#include "gameCreator.h"

class HydraGameCreator : public GameCreator {
  public:
    virtual std::shared_ptr<Game> newGame ( GameOptions gameOptions ) override;
};

#endif
