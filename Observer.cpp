#include "doctest.h"

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>


//Zrobic tutaj prosty przyklad

// 3 miejsca w ktorych jest potrzebna notyfikacja
// i dodac recznie 4 miejsce albo inny paramter

// potem 3 miejsca z observerem i dodanie 4 miejsca - jakie jest proste

//pozniej sie przelaczyc na rzecyzwisty przyklad

// pokazac ze mozna zmienic w czasie wykonywania
// pokzazc ze mozna robic rozne histerezy (setChanged)                    PUSH vs PULL


//Observer
class Display
{
public:
    virtual ~Display() {}
    virtual void notify(double value) = 0;
};

class LcdScreen : public Display
{
public:
    void notify(double value) override
    {
        std::cout << "Updating LcdScreen\n";
    }
};

class Buzzer : public Display
{
public:
    void notify(double value) override
    {
        std::cout << "Triggering Buzzer\n";
    }
};

class SmsNotification: public Display
{
public:
    void notify(double value) override
    {
        std::cout << "Sending SmsNotification\n";
    }
};

class Stock
{
private:
    double value;
    std::vector<std::shared_ptr<Display>> observers;
public:
    void valueChanged()
    {
        //this method is called periodically - we don't care how
        for(auto observer : observers)
        {
            observer->notify(value);
        }
    }

    void attach(std::shared_ptr<Display> newObserver)
    {
        observers.push_back(newObserver);
    }
    void detach(std::shared_ptr<Display> observer)
    {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void setValue(double newValue)
    {
        //just for test purposes
        value = newValue;
        valueChanged();
    }
};

TEST_SUITE_BEGIN("Observer.cpp");

///////////////////////////////////////////////////////////////////////////////
//
//  Now the Sun of the Design Patterns shine...
//
///////////////////////////////////////////////////////////////////////////////

TEST_CASE("Typical usage of an observer")
{
    auto lcd  = std::make_shared<LcdScreen>();
    auto buzz = std::make_shared<Buzzer>();
    auto sms  = std::make_shared<SmsNotification>();

    Stock motoStock;

    motoStock.attach(lcd);
    motoStock.attach(buzz);
    motoStock.attach(sms);

    motoStock.setValue(1.0);
//after few hours
    motoStock.setValue(5.0);

    motoStock.detach(buzz);

    motoStock.setValue(5.1);
}

TEST_SUITE_END();
