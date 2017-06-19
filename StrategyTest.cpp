#include "doctest.h"

#include <iostream>
#include <memory>

// based on http://www.bogotobogo.com/DesignPatterns/strategy.php

//Strategy
class SortBehavior
{
public:
    virtual ~SortBehavior() { }
    virtual void sort() const = 0;
};

class ByDate: public SortBehavior
{
public:
    virtual void sort() const override
    {
        std::cout << "ByDate sort()\n";
    }
};

class ByAuthor: public SortBehavior {
public:
    virtual void sort() const override
    {
        std::cout << "ByAuthor sort()\n";
    }
};

class ByGenre: public SortBehavior
{
public:
    virtual void sort() const override
    {
        std::cout << "ByGenre sort()\n";
    }
};

// Context
class Collection
{
private:
    SortBehavior* m_sort;
    std::string m_collectionName;
public:
    Collection(const std::string& name) : m_sort(nullptr), m_collectionName(name) {}
    void set_sort(SortBehavior* s)
    {
        m_sort = s;
    }
    void sort() const
    {
        std::cout << "Sorting " << m_collectionName << " with ";
        m_sort->sort();
    }
};

TEST_SUITE_BEGIN("StrategyTest.cpp");

TEST_CASE("Encapsulate a family of algorithm into their own set of classes")
{
    ByDate      sortByDate;
    ByAuthor    sortByAuthor;

    Collection  books("books");
    Collection  newspapers("newspapers");

    books.set_sort(&sortByAuthor);
    books.sort();

    newspapers.set_sort(&sortByDate);
    newspapers.sort();
}

TEST_CASE("Change behavior at run time")
{
    ByAuthor    sortByAuthor;
    ByGenre     sortByGenre;

    Collection  books("books");

    books.set_sort(&sortByAuthor);
    books.sort();

    books.set_sort(&sortByGenre);
    books.sort();
}

TEST_SUITE_END();
