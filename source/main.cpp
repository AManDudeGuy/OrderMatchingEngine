#include <iostream>
#include <string>
#include <fstream>

const int BUY = 0;
const int SELL = 1;

struct OrderMessage{
    std::string_view ticker{};
    int buyOrSell{};
    int price{}; // in cents to avoid floating point errors
    int amountOfShares{};
};

int main(int argc, char* argv[]){


    return 0;
}
help
// TODO:
// Implement storage of buy sell orders
// Implement order execution logic
// Implement recieving of bids and asks and acknowledgements
// Implement order cancelation
// Implement per stock
// Make it FAST
