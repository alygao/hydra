#include "hydraGameCreator.h"
#include "beginnerComputerPlayingStrategy.h"
#include "advancedComputerPlayingStrategy.h"
#include "hydraGame.h"
#include "textDisplay.h"
#include "guiDisplay.h"
#include "gameOptions.h"

std::shared_ptr<Game> HydraGameCreator::newGame ( GameOptions gameOptions ) {
  std::shared_ptr<Game> game;
  std::shared_ptr<GameDisplay> display;
  std::shared_ptr<GameStrategy> computerPlayerStrategy = nullptr;

  if ( gameOptions.displayType == GameDisplayType::Text ) {
    display = std::make_shared<TextDisplay> ( std::cin, std::cout, std::cerr );
  } else {
    display = std::make_shared<GUIDisplay> ( 2 );
  }
  if ( gameOptions.computerPlayerLevel == 1 ) {
      computerPlayerStrategy = std::make_shared<BeginnerComputerPlayingStrategy>();
  } else if ( gameOptions.computerPlayerLevel > 1 ) {
      computerPlayerStrategy = std::make_shared<AdvancedComputerPlayingStrategy>();
  }
  game = std::make_shared<HydraGame> ( display, gameOptions, computerPlayerStrategy );
  return game;
}
