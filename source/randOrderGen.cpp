#include <fstream>
#include <iostream>
#include <random>
#include <vector>
#include <chrono>
#include <iomanip>

const int BUY = 0;
const int SELL = 1;

struct OrderMessage{
    std::string_view ticker{};
    int buyOrSell{};
    int price{}; // in cents to avoid floating point errors
    int amountOfShares{};
};

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

void printOrderToFile(std::ofstream& outFile, OrderMessage om){
    outFile << om.ticker << ","
    << om.buyOrSell << ","
    << om.price << ","
    << om.amountOfShares << "\n";
}


// Generates random order messages to test the matching engine on
int main(int argc, char* argv[]){

    const int numMessages = 10'000'000;
    auto start = std::chrono::steady_clock::now();

    std::ofstream orderMessageFile(argv[1]);
    std::ifstream tickerFile(argv[2]);

    if(!orderMessageFile || !tickerFile){
        std::cout << "File init error" << "\n\n\n";
        return 1;
    }
    
    // stores tickers from file in memory
    std::vector<std::string> tickerArr{};
    std::string tempVar{};
    while(getline(tickerFile, tempVar, ',')){
        tickerArr.push_back(tempVar);
    }
    tickerFile.close();
    

   for(int i = 0; i < numMessages; i++){
        printOrderToFile(orderMessageFile, randomOrderMessage(tickerArr));
    }

    std::cout << "Generated file\n";
    orderMessageFile.close();

    auto stop = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Program run-time (seconds): " << std::setprecision(6) << duration.count()/1000000.0 << "s\n\n\n";
    return 0;
}