#include "doctest.h"
#include <iostream>
#include <memory>

//Example presenting Astro Subscriber Interface

namespace RawDesign {

class Window
{
public:
    virtual ~Window() = default;
    virtual void show() = 0;
};

class WindowWithScrollbar : public Window
{
public:
    void show() { std::cout << "Showing Window with scrollbar\n"; }
};

class WindowWithDate : public Window
{
public:
    void show() { std::cout << "Showing Window with time & date\n"; }
};

class WindowWithScrollbarAndDate : public Window
{
public:
    void show() { std::cout << "Showing Window with scrollbar and time & date\n"; }
};

TEST_CASE("Check this awesome subscriber interface!!!" * doctest::skip())
{
    std::unique_ptr<Window> currentDisplay;

    currentDisplay.reset(new WindowWithScrollbar);
    currentDisplay->show();

    currentDisplay.reset(new WindowWithDate);
    currentDisplay->show();

    currentDisplay.reset(new WindowWithScrollbarAndDate);
    currentDisplay->show();

    // What is wrong with it?
    // 1) hard to extend it with new decoration ( e.q. CurrentChannel)
    // 2) fixed calls' order (e.q. a->b->c and to have b->c->a we need additional classes
    // 3) fixed number of decorations (what if someone would like to has triple cheese in pizza?)
}

}

///////////////////////////////////////////////////////////////////////////////
//
//  Now the Sun of the Design Patterns shine...
//
///////////////////////////////////////////////////////////////////////////////

namespace DecoratorDesign {

class Window
{
public:
    virtual ~Window() = default;
    virtual void show() = 0;
};

//Object that is least common denominator
class SubscriberLcd : public Window
{
public:
    void show() { std::cout << "Showing subscriber lcd.\n"; }
};

class Decorator : public Window
{
    Window* base;
public:
    Decorator(Window* parent) : base(parent) {}
    void show()
    {
        base->show();
    }
};

class Scrollbar : public Decorator
{
public:
    Scrollbar(Window* parent) : Decorator(parent) {}
    void show() override
    {
        Decorator::show();
        std::cout << "Adding scrollbar\n";
    }
};

class CurrentDate : public Decorator
{
public:
    CurrentDate(Window* parent) : Decorator(parent) {}
    void show() override
    {
        Decorator::show();
        std::cout << "Adding current date\n";
    }
};

class WarningMessage : public Decorator
{
public:
    WarningMessage(Window* parent) : Decorator(parent) {}
    void show() override
    {
        std::cout << "Welcome!\n";
        Decorator::show();
    }
};

TEST_CASE("Typical usage of an decorator")
{
    SubscriberLcd subLcd;
    Scrollbar scrollbar(&subLcd);
    CurrentDate currentDate(&scrollbar);
    WarningMessage warningMessage(&currentDate);

    Window* astroLcd = &warningMessage;

    astroLcd->show();
}

}
