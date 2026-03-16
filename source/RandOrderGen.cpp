#include "RandOrderGen.hpp"
#include "RandomOrder.hpp"
#include <fstream>
#include <iostream>
#include <random>
#include <vector>
#include <chrono>
#include <iomanip>

void printOrderToFile(std::ofstream& outFile, OrderMessage om){
    outFile << om.ticker << ","
    << om.buyOrSell << ","
    << om.price << ","
    << om.amountOfShares << "\n";
}


// Generates random order messages to test the matching engine on
int main(int argc, char* argv[]){

    const int numMessages = std::stoi(argv[3]);
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
    

   for(int i = 0; i < numMessages - 1; i++){
        printOrderToFile(orderMessageFile, randomOrderMessage(tickerArr));
    }
    OrderMessage last = randomOrderMessage(tickerArr);
    orderMessageFile << last.ticker << ","
    << last.buyOrSell << ","
    << last.price << ","
    << last.amountOfShares;

    std::cout << "Generated file\n";
    orderMessageFile.close();

    auto stop = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Program run-time (seconds): " << std::setprecision(6) << duration.count()/1000000.0 << "s\n\n\n";
    return 0;
}