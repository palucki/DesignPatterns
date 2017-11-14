#include <iostream>
#include <fstream>

#include "doctest.h"
#include "nlohmann/json.hpp"

// Adapter example
// When you decide to replace your old, time-worn library with something more up-to-date
// you face the problem of replacing all library calls throughout the code.
// The solution to this is to use Adapter Design Pattern, and wrap new library into old interface.

class jsonAdapter : private nlohmann::json
{
public:
    int size()
    {
        return 100;
    }
};


TEST_CASE("A lot of library calls")
{
    using json = nlohmann::json;

    std::ifstream test_data("Adapter/data1.json");
    json cds_by_artists;
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

