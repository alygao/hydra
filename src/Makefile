CXX = g++                   # compiler
CXXFLAGS = -std=c++14 -g -Wall -MMD # compiler flags
OBJECTS = main.o spade.o heart.o diamond.o club.o suit.o cardProcessor.o cardValueProcessor.o gameCreator.o hydraGameCreator.o gameStrategy.o beginnerComputerPlayingStrategy.o advancedComputerPlayingStrategy.o game.o hydraGame.o card.o gameDisplay.o textDisplay.o guiDisplay.o x11Window.o pokerCardPixelLibrary.o pokerCardPixel.o head.o player.o gameSubject.o util.o jokerCard.o    # object files for the executable
EXEC = hydra                 # executable name
DEPENDS = ${OBJECTS:.o=.d}  # substitute ".o" with ".d"

########## Targets ##########

.PHONY : all clean          # not file names

${EXEC} : ${OBJECTS}                # link step executable
	${CXX} ${CXXFLAGS} $^ -o $@ -lX11 -lXpm     # additional object files before $^

-include ${DEPENDS}         # include *.d files containing program dependences

clean :                     # remove files that can be regenerated
	rm -f ${DEPENDS} ${OBJECTS} ${EXEC} 
