#include <iostream>
#include <string>
#include <fstream>

// Order Matching Engine

struct OrderMessage{
    std::string ticker{};
    std::string buyOrSell{};
    double price{};
    int amountOfShares{};
};

int main(){

    std::fstream file("example.txt", std::ios::out);

    for(int i = 0; i < 10000; i++){
        file << i << "\n";
    }

    file.close();

    return 0;
}