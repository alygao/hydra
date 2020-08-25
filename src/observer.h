#ifndef __OBSERVER_H__
#define __OBSERVER_H__
#include <iostream>
#include "gameSubject.h"

class Observer {
  public:
    // Pass the Subject that called the notify method.
    virtual void notify( GameSubject & whoNotified ) = 0; 
    virtual ~Observer() = default;
};

#endif
