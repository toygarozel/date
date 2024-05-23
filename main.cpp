#include "date.hpp"
#include <ctime>
#include <iostream>

int main()
{
    try {
        std::time_t now = std::time(nullptr);
        date::Date today(now);
        std::cout << today;
    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
    }
}