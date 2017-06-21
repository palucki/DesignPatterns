#include "doctest.h"

#include <iostream>


//Zrobic tutaj prosty przyklad

// 3 miejsca w ktorych jest potrzebna notyfikacja
// i dodac recznie 4 miejsce albo inny paramter

// potem 3 miejsca z observerem i dodanie 4 miejsca - jakie jest proste

//pozniej sie przelaczyc na rzecyzwisty przyklad

// pokazac ze mozna zmienic w czasie wykonywania
// pokzazc ze mozna robic rozne histerezy (setChanged)                    PUSH vs PULL


//cos co uzytkownik bedzie mogl rozwijac sam

// GIELDA JAKO PRZYKLAD !!!!!!!!!!!
// notyfikacja na ekranach
// notyfikacja na lampce / glosniku przy kazdej zmianie
// notyfikacja na telefonach uzytkownikow
// notyfikacja mailem do zainteresownaych

//class Display
//{
//    virtual void updateData(double value) = 0;
//};

class Stock
{
private:
    std::string name;
    double value;
public:
    void updateValue(double newValue)
    {
        //this method is called periodically - we don't care how
        value = newValue;


        //update all displays
    }
};



TEST_SUITE_BEGIN("Observer.cpp");



///////////////////////////////////////////////////////////////////////////////
//
//  Now the Sun of the Design Patterns shine...
//
///////////////////////////////////////////////////////////////////////////////


TEST_SUITE_END();
