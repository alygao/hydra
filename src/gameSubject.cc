#include "gameSubject.h"
#include "observer.h"

GameSubject::~GameSubject() { }

void GameSubject::attach(std::shared_ptr<Observer> o) { observers.emplace_back(o); }

void GameSubject::notifyObservers() {
    for (auto &ob : observers) ob->notify( *this );
}
