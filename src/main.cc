#include "hydraGameCreator.h"
#include "gameOptions.h"
#include "game.h"
#include "util.h"

void usage ( ) {
    std::cout << "\nUsage:" << std::endl;
    std::cout << "  hydra [-testing] [-enhancement|-enablebonus] [-display=GUI|TEXT] [-computerPlayerLevel=n] [-cpLevel=n] [-computerPlayerActionElapseTime=ms] [[-cpTime=ms] [-cardsPerPlayer=n]" << std::endl << std::endl;
    std::cout << "  Flags (Latter flags overwrite previous same flags. Flag name and value are case insensitive.):" << std::endl;
    std::cout << "   -testing:\tenables testing mode." << std::endl;
    std::cout << "   -enhancement:\tenables all enhancement/bonus features: GUI display, computer players (level 1) with action elapse time of 1000 ms." << std::endl;
    std::cout << "   -enablebonus:\tsame as -enhancement flag." << std::endl;
    std::cout << "   -display:\tsets basic TEXT display or GUI display. Valid values are: Text or GUI. Defaults to Text." << std::endl;
    std::cout << "   -computerPlayerLevel:\tsets the level of the computer player. The value must be greater than 0. Default to 1, i.e., no computer player." << std::endl;
    std::cout << "   -cpLevel:\tsame as -computerPlayerLevel flag." << std::endl;
    std::cout << "   -computerPlayerActionElapseTime:\tsets time in milliseconds for simulating how long computer player actions take. It takes into effective when the computerPlayerLevel > 0. The value must be a positive number. Default to 1000 , i.e., 1 second. Zero means no wait." << std::endl;
    std::cout << "   -cpTime:\tsame as -computerPlayerActionElapseTime flag." << std::endl;
    std::cout << "   -cardsPerPlayer:\tnumber of cards allocated to each player, in range between 5 to 54. Default to 54 cards per player. This value is used only in testing mode." << std::endl;
}

bool processArguments ( int argc, char*argv[], GameOptions & options ) {
 for (int i = 1; i < argc; i++) {
  std::string arg0{argv[i]};
        std::string arg = to_uppercase(arg0);
        if ( arg == "-TESTING" ) {
            options.testingMode = true;
        } else if ( arg == "-ENHANCEMENT" || arg == "-ENABLEBONUS" ) {
            options.displayType = GameDisplayType::GUI;
            options.computerPlayerLevel = 1;
            options.computerPlayerActionElapseTime = 1000;
        } else {
            size_t pos = arg.find ( "=", 0 );
            if ( pos == std::string::npos ) {
                std::cerr << "Invalid flag: " << arg0 << std::endl;
                return false;
            }
            std::string key = arg.substr ( 0, pos );
            std::string value = arg.substr ( pos + 1 );
            if ( key == "-COMPUTERPLAUYERLEVEL" || key == "-CPLEVEL" ) {
                try {
                    int level = std::stoi ( value );
                    if ( level < 1 ) {
                        std::cerr << "Invalid computer player level (the value must be greater than 0)!" << std::endl;
                        return false;
                    } else {
                        options.computerPlayerLevel = level;
                    }
                } catch ( std::exception & ) {
                    std::cerr << "Invalid computer player level!" << std::endl;
                    return false;
                }
            } else if ( key == "-COMPUTERPLAYERACTIONELAPSETIME" || key == "-CPTIME" ) {
                try {
                    int elapseTime = std::stoi ( value );
                    if ( elapseTime < 0 ) {
                        std::cerr << "Invalid computer player action elapse time (the value must be a nonegative number)!" << std::endl;
                        return false;
                    } else {
                        options.computerPlayerActionElapseTime = elapseTime;
                    }
                } catch ( std::exception & ) {
                    std::cerr << "Invalid computer player level!" << std::endl;
                    return false;
                }
            } else if ( key == "-DISPLAY" ) {
                value = to_uppercase ( value );
                if ( value == "TEXT" ) {
                    options.displayType = GameDisplayType::Text;
                } else if ( value == "GUI" ) {
                    options.displayType = GameDisplayType::GUI;
                } else {
                    std::cerr << "Invalid display type (must be either TEXT or GUI) !" << std::endl;
                    return false;
                }
            } else if ( key == "-CARDSPERPLAYER" ) {
                try {
                    int numOfCardPerPlayer = std::stoi ( value );
                    if ( numOfCardPerPlayer < 5 || numOfCardPerPlayer > 54 ) {
                        std::cerr << "Invalid number of cards allocated to each player (the value must be between 5 to 54)!" << std::endl;
                        return false;
                    } else {
                        options.numOfCardPerPlayer = numOfCardPerPlayer;
                    }
                } catch ( std::exception & ) {
                    std::cerr << "Invalid computer player level!" << std::endl;
                    return false;
                }
            } else {
                std::cerr << "Invalid flag: " << arg0 << std::endl;
                return false;
            }
        }
    }
    return true;
}

void printGameOptions ( const GameOptions & options ) {
    std::cout << "Game starts:" << std::endl;
    std::cout << "\tTesting mode: " << ( options.testingMode ? "Yes" : "No" ) << std::endl;
    std::cout << "\tDisplay type: " << ( options.displayType == GameDisplayType::Text ? "Text" : "GUI" ) << std::endl;
    std::cout << "\tComputer player: " << ( options.computerPlayerLevel > 0 ? "Level " + std::to_string (options.computerPlayerLevel) : "No" ) << std::endl;
    if ( options.computerPlayerLevel > 0 ) {
        std::cout << "\tComputer player action elapse time: " << options.computerPlayerActionElapseTime << std::endl;
    }
    std::cout << std::endl;
    if ( options.displayType == GameDisplayType::GUI ) {
        std::cout << "Please operate on GUI window . . ." << std::endl;
    }
}

int main ( int argc, char * argv[] ) {
    GameOptions options;
    if ( !processArguments ( argc, argv, options ) ) {
        usage();
        return 1;
    }
    printGameOptions ( options );

    std::shared_ptr<GameCreator> gameCreator = std::make_shared<HydraGameCreator> ( );
    std::shared_ptr<Game> game = gameCreator->newGame ( options );
    game->startGame();

}
