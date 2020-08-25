#ifndef __GAMESUBJECT_H__
#define __GAMESUBJECT_H__
#include <iostream>
#include <vector>
#include <memory>

class Observer;
class GameInfo;

class GameSubject {
    std::vector<std::shared_ptr<Observer>> observers;

  public:
    virtual ~GameSubject ( );
    void attach(std::shared_ptr<Observer> o);  
    void notifyObservers();
    virtual GameInfo getGameInfo() const = 0;

};

#endif
