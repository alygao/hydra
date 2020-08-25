#ifndef __X11WINDOW_H__
#define __X11WINDOW_H__

#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/xpm.h>
#include <map>

class X11Window {
  private:
    virtual void clearAnswer ( ) const = 0;

    Display *disp;
    Window win;
    int screen;
    GC gc;
    unsigned long colours[10];
    XFontStruct * font;

    int width = 600, height = 300;

    std::string handleInputEvent ( int x, int y ) const;
    std::map<std::string, XImage *> images;
    void loadImages ( );
    virtual void refreshDisplay() const = 0;
    virtual void drawImageIfNotExist ( const std::string & imageName, int x, int y ) const = 0;

  protected:
    std::string getInput ( int x, int y ) const;

  public:
    X11Window ( int width = 1600, int height = 900 );
    virtual ~X11Window ( );

    enum {White=0, Black, Red, Green, Blue, Yellow, GameBackgroundColour, LineColour, FrameColour, Grey};

    void fillRectangle(int x, int y, int width, int height, int colour=Black) const;
    void drawRectangle(int x, int y, int width, int height, int colour) const;
    void drawString(int x, int y, const std::string & msg, int colour = Yellow) const;
    void displayImage ( const std::string & imageName, int x, int y, int width = 0, int height = 0 ) const;
};

#endif
