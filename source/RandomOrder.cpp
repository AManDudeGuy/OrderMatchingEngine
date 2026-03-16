#include "RandomOrder.hpp"
#include <vector>
#include <random>
#include "Common.hpp"


OrderMessage randomOrderMessage(const std::vector<std::string>& tickerArr){

    // random generators for OrderMessage members
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> tickerDistrib(0, tickerArr.size() - 1);
    static std::uniform_int_distribution<> buySellDistrib(0, 1);
    static std::uniform_int_distribution<> priceDistrib(0, 100000);
    static std::uniform_int_distribution<> amountDistrib(1, 100);

    return OrderMessage {tickerArr[tickerDistrib(gen)], buySellDistrib(gen), priceDistrib(gen), amountDistrib(gen)};
}