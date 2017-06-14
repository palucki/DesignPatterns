#include <iostream>
#include "doctest.h"

int factorial(int number) { return number <= 1 ? number : factorial(number - 1) * number; }


TEST_SUITE_BEGIN("StrategyTest.cpp");

TEST_CASE("testing the factorial function")
{
    CHECK(factorial(1) == 1);
    CHECK(factorial(2) == 2);
    CHECK(factorial(3) == 6);
    CHECK(factorial(10) == 3628800);
}

TEST_CASE("testing the factorial function2")
{
    CHECK(factorial(1) == 1);
    CHECK(factorial(2) == 2);
    CHECK(factorial(3) == 6);
    CHECK(factorial(10) == 3628800);
}

TEST_SUITE_END();
