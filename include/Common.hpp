#ifndef COMMON_HPP
#define COMMON_HPP

#include <string>

constexpr int BUY = 0;
constexpr int SELL = 1;
constexpr int MAX_NUMBER_OF_ORDERS_STORED = 100'000; // not ideal but i dont have much ram :(

struct OrderMessage{
    std::string ticker{};
    int buyOrSell{};
    int price{}; // in cents to avoid floating point errors
    int amountOfShares{};
};


#endif