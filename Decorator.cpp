#include "doctest.h"
#include <iostream>
#include <memory>

TEST_SUITE_BEGIN("Decorator.cpp");
//Example presenting Astro Subscriber Interface
// Window
// WindowWithScrollbar
// WindowWithDate
// WindowWithScrollbarAndDate
// WindowWithScrollbarAndChannelNumber

namespace RawDesign {

class Window
{
public:
    virtual ~Window() {}
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
//powiedziec tylko w prezentacji ze decoratora sie uzywa na przyklad do strumieni i podac te przyklady oraz ze na przyklad do ramek by to moglo byc?
// powiedziec ze tu zeby wybrac KIEDY sie cos odpali to musi byc zahardkodowane
//
// fajny przyklad: mamy jakis zbior danych w arkuszu, na ktoyr stosujemy formatowanie warunkowe i tak na pryzklad pogrubaimy, podkreslamy, zmieniamy kolor w zaleznosci od danych
// kazdy taki element filtra to dekorator


TEST_CASE("Check this awesome subscriber interface!!!" * doctest::skip())
{
    std::unique_ptr<Window> currentDisplay;

    currentDisplay.reset(new WindowWithScrollbar);
    currentDisplay->show();

    currentDisplay.reset(new WindowWithDate);
    currentDisplay->show();

    currentDisplay.reset(new WindowWithScrollbarAndDate);
    currentDisplay->show();

    //i co jesli teraz mamy dodac np Channel Number do ekranu?
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
    virtual ~Window() {}
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

class WelcomeMessage : public Decorator
{
public:
    WelcomeMessage(Window* parent) : Decorator(parent) {}
    void show() override
    {
        std::cout << "Welcome!\n";
        Decorator::show();
    }
};

TEST_CASE("Typical usage of an decorator")
{
    Window* astroLcd = new WelcomeMessage(new CurrentDate(new Scrollbar(new SubscriberLcd)));

    astroLcd->show();
}


}

TEST_SUITE_END();
