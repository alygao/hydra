#include <unistd.h>
#include <X11/XKBlib.h>
#include <algorithm>

#include "x11Window.h"

static const std::string IMAGE_FOLDER = "images/";
static const std::string IMAGE_EXTENSION = ".xpm";


X11Window::X11Window ( int width, int height ) : width{width}, height{height} {
    disp = XOpenDisplay(NULL);
    if ( disp == NULL ) {
        std::cerr << "Cannot open display" << std::endl;
        exit(1);
    }
    screen = DefaultScreen(disp);
    win = XCreateSimpleWindow(disp, RootWindow(disp, screen), 0, 0, width, height, 1, BlackPixel(disp, screen), WhitePixel(disp, screen));
    XSelectInput(disp, win, ExposureMask | KeyPressMask);
    XMapRaised(disp, win);

    Pixmap pix = XCreatePixmap ( disp, win, width, height, DefaultDepth(disp,DefaultScreen(disp)));
    gc = XCreateGC(disp, pix, 0,(XGCValues *)0);

    XFlush(disp);
    XFlush(disp);

    // Set up colours.
    XColor xcolour;
    Colormap cmap;
    char color_vals[7][10]={"white", "black", "red", "green", "blue", "yellow"};
    cmap = DefaultColormap ( disp, DefaultScreen(disp) ) ;
    for ( int i=0; i < 6; ++i ) {
        XParseColor ( disp, cmap, color_vals[i], &xcolour );
        XAllocColor ( disp, cmap, &xcolour);
        colours[i] = xcolour.pixel;
    }

    // Set additional colours
    colours[6] = 0x023B0E;
    colours[7] = 0x345220;
    colours[8] = 0xD9B32A;
    colours[9] = 0x7F7F7F;

    XSetForeground ( disp, gc, colours[Black] );

    // Make window non-resizeable.
    XSizeHints hints;
    hints.flags = (USPosition | PSize | PMinSize | PMaxSize );
    hints.height = hints.base_height = hints.min_height = hints.max_height = height;
    hints.width = hints.base_width = hints.min_width = hints.max_width = width;
    XSetNormalHints(disp, win, &hints);

    XSynchronize(disp,True);

    loadImages();

    usleep(1000);
}

X11Window::~X11Window() {
    for ( auto p : images ) {
        XDestroyImage ( p.second );
    }
    XClearWindow ( disp, win );
    XFreeGC (disp, gc );
    XDestroyWindow ( disp, win );
    XCloseDisplay ( disp );
}

void X11Window::loadImages ( ) {
    fillRectangle ( 0, 0, 1600, 900, GameBackgroundColour );
    int imageSeqNo = 1;
    XImage *img;
    std::string suit = "HSDC";
    for ( int i = 1; i <= 13; i ++ ) {
        std::string imageName;
        if ( i == 1 ) {
            imageName = "A";
        } else if ( i == 11 ) {
            imageName = "J";
        } else if ( i == 12 ) {
            imageName = "Q";
        } else if ( i == 13 ) {
            imageName = "K";
        } else {
            imageName = std::to_string ( i );
        }
        for ( int j = 0; j < 4; j ++ ) {
            fillRectangle ( 600, 400, 500, 100, GameBackgroundColour );
            drawString ( 600, 450, "Loading images: " + std::to_string ( imageSeqNo ++ ) + " of 55, please wait . . .", Yellow );
            std::string imageFilename = IMAGE_FOLDER + imageName + suit.substr ( j, 1 ) + IMAGE_EXTENSION;
            if ( XpmReadFileToImage (disp, imageFilename.c_str(), &img, NULL, NULL )) {
                std::cerr << "Error reading file:" << imageFilename << std::endl;
            } else {
                images[imageFilename] = img;
            }
        }
    }
    std::string additionalImageNames[] = { "card_back", "Joker", "hydra_logo" };
    for ( int i = 0; i < 3; i ++ ) {
        fillRectangle ( 600, 400, 500, 100, GameBackgroundColour );
        drawString ( 600, 450, "Loading images: " + std::to_string ( imageSeqNo ++ ) + " of 55, please wait . . .", Yellow );
        std::string imageFilename = IMAGE_FOLDER + additionalImageNames[i] + IMAGE_EXTENSION;
        if ( XpmReadFileToImage (disp, imageFilename.c_str(), &img, NULL, NULL )) {
            std::cerr << "Error reading file:" << imageFilename << std::endl;
        } else {
            images[imageFilename] = img;
        }
    }
    fillRectangle ( 600, 400, 500, 100, GameBackgroundColour );
    drawString ( 600, 450, "Loading images: completed!", Yellow );
}

void X11Window::fillRectangle ( int x, int y, int width, int height, int colour ) const {
    XSetForeground ( disp, gc, colours[colour] );
    XFillRectangle ( disp, win, gc, x, y, width, height );
    XSetForeground ( disp, gc, colours[Black] );
}

void X11Window::drawRectangle ( int x, int y, int width, int height, int colour ) const {
    XSetForeground ( disp, gc, colours[colour] );
    XDrawRectangle ( disp, win, gc, x, y, width, height );
    XSetForeground ( disp, gc, colours[Black] );
}

void X11Window::drawString ( int x, int y, const std::string & msg, int colour ) const {
    XSetForeground ( disp, gc, colours[colour] );
    XDrawString ( disp, win, gc, x, y, msg.c_str (), msg.length() );
}

void X11Window::displayImage ( const std::string & imageName, int x, int y, int width, int height ) const {

    std::string imageFilename = IMAGE_FOLDER + imageName + IMAGE_EXTENSION;
    auto imageIt = images.find ( imageFilename );
    if ( imageIt != images.end() ) {
        XImage * img = imageIt->second;
        if ( width == 0 ) {
            width = img->width;
        }
        if ( height == 0 ) {
            height = img->height;
        }
        XPutImage(disp, win, gc, img, 0, 0, x, y, width, height );
    } else {
        drawImageIfNotExist ( imageName, x, y );
    }
}

std::string X11Window::getInput ( int x, int y ) const {
    XSelectInput ( disp, win, ExposureMask | ButtonPressMask | KeyPressMask | KeyReleaseMask );

    clearAnswer();
    std::string input = handleInputEvent( x, y );

    XSelectInput ( disp, win, 0 );

    return input;
}

std::string X11Window::handleInputEvent ( int x, int y ) const {
    /* this structure will contain the event's data, once received. */
    XEvent an_event;
    std::string answer = "";
    unsigned long previousExposeEventSerial = 0;

    /* enter an "endless" loop of handling events. */
    while ( true ) {
        XNextEvent ( disp, &an_event );
        switch ( an_event.type ) {
        case Expose:
            /* handle this event type... */
            if ( an_event.xexpose.count > 0 || previousExposeEventSerial == an_event.xexpose.serial ) {
                break;
            }
            previousExposeEventSerial = an_event.xexpose.serial;
            refreshDisplay();
            clearAnswer();
            drawString ( x, y, answer );
            break;
        case ButtonPress:
            break;
        case KeyPress:
            break;
        case KeyRelease:
            KeySym key_symbol;
            key_symbol = XkbKeycodeToKeysym ( disp, an_event.xkey.keycode, 0, 0 );
            if ( key_symbol == XK_Return || key_symbol == XK_KP_Enter ) {
                return answer;
            } else if ( key_symbol == XK_BackSpace ) {
                if ( answer.size() > 0 ) {
                    answer = answer.substr ( 0, answer.size() - 1 );
                    clearAnswer();
                    drawString ( x, y, answer );
                }
            }
            if ( ( key_symbol >= XK_0 && key_symbol <= XK_9 ) ||
                 ( key_symbol >= XK_A && key_symbol <= XK_Z ) ||
                 ( key_symbol >= XK_a && key_symbol <= XK_z ) ||
                   key_symbol == XK_space ) {
                char * keySymbolString = XKeysymToString ( key_symbol );
                answer += ( key_symbol == XK_space ? ' ' : keySymbolString[0] );
                drawString ( x, y, answer );
            }
            
            break;
        default: // unknown event type - ignore it.
            break;
        }
    }
}
