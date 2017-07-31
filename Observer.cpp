#include "doctest.h"

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>


namespace RawDesign {

class LcdScreen
{
public:
    void notify(double value)
    {
        std::cout << "Updating LcdScreen\n";
    }
};

class SmsNotification
{
public:
    void notify(double value)
    {
        std::cout << "Sending SmsNotification\n";
    }
};

class Buzzer
{
public:
    void notify(double value)
    {
        if(enabled)
        {
            std::cout << "Triggering Buzzer\n";
        }
    }
    void enable(bool value)
    {
        enabled = value;
    }
private:
    bool enabled = false;
};

class Stock
{
private:
    double value;
    LcdScreen lcd;
    Buzzer buzz;
    SmsNotification sms;
public:
    void valueChanged()
    {
        //this method is called periodically - we don't care how
        lcd.notify(value);
        sms.notify(value);
        buzz.notify(value);
    }

    void setValue(double newValue)
    {
        //just for test purposes
        value = newValue;
        valueChanged();
    }

    void enableBuzzer(bool value)
    {
        buzz.enable(value);
    }
};

TEST_CASE("Raw notifications" * doctest::skip())
{
    Stock motoStock;

    motoStock.enableBuzzer(true);
    motoStock.setValue(1.0);
//after few hours
    motoStock.enableBuzzer(false);
    motoStock.setValue(5.0);
}

}

///////////////////////////////////////////////////////////////////////////////
//
//  Now the Sun of the Design Patterns shine...
//
///////////////////////////////////////////////////////////////////////////////


namespace DesignPatterns {

//Observer
class NotificationChannel
{
public:
    virtual ~NotificationChannel() {}
    virtual void notify(double value) = 0;
};

class LcdScreen : public NotificationChannel
{
public:
    void notify(double value) override
    {
        std::cout << "Updating LcdScreen\n";
    }
};

class Buzzer : public NotificationChannel
{
public:
    void notify(double value) override
    {
        std::cout << "Triggering Buzzer\n";
    }
};

class SmsNotification: public NotificationChannel
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
    std::vector<std::shared_ptr<NotificationChannel>> observers;
public:
    void valueChanged()
    {
        //this method is called periodically - we don't care how
        for(auto observer : observers)
        {
            observer->notify(value);
        }
    }

    void attach(std::shared_ptr<NotificationChannel> newObserver)
    {
        observers.push_back(newObserver);
    }
    void detach(std::shared_ptr<NotificationChannel> observer)
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

//when someone is annoyed with buzzer
    motoStock.detach(buzz);

    motoStock.setValue(5.1);
}

}
