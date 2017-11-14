#include <iostream>
#include <fstream>

#include "doctest.h"
#include "MinimalJson.h"
#include "nlohmann/json.hpp"

// Adapter example
// When you decide to replace your old, time-worn (or maybe even not working) library
// with something more up-to-date you face the problem of replacing all library calls throughout the code.
// The solution to this is to use Adapter Design Pattern, and wrap new library into old interface.

class MinimalJsonAdapter : public MinimalJson, private nlohmann::json
{
public:
    MinimalJsonAdapter(const std::string& filename) : MinimalJson(filename)
    {

    }
    virtual size_t size() override
    {
        return 100;
    }
private:
//    nlohmann::json& adaptee;
};


TEST_CASE("A lot of library calls")
{
    std::ifstream test_data("Adapter/data1.json");
    nlohmann::json cds_by_artists;
    test_data >> cds_by_artists;

//    jsonAdapter music_collection;

    std::cout << "Artists in collection: " << cds_by_artists.size() << "\n";
    for(auto artist : cds_by_artists)
    {
        std::cout << artist["name"] << " has " << artist["albums"].size() << " albums:"<< std::endl;

        for(auto album : artist["albums"])
        {
            std::cout << '\t' << album["title"] << " contains " << album["songs"].size() << " songs."<< std::endl;

            std::cout << "\t\t";
            for(auto song : album["songs"])
            {
                std::cout << song["title"] << ", ";
            }
            std::cout << "\n";
        }
    }

}

