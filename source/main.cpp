#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <sstream>
#include <format>
#include <algorithm>
#include <chrono>

// Very Naive order matching engine that works only for a single ticker

const int BUY = 0;
const int SELL = 1;
const int MAX_NUMBER_OF_ORDERS_STORED = 100000; // not ideal but i dont have much ram :(

struct OrderMessage{
    std::string ticker{};
    int buyOrSell{};
    int price{}; // in cents to avoid floating point errors
    int amountOfShares{};
};


// stack implementation
struct OrderStore{
    int buyNum{};
    int sellNum{};
    std::array<OrderMessage, MAX_NUMBER_OF_ORDERS_STORED/2> buyStore{};
    std::array<OrderMessage, MAX_NUMBER_OF_ORDERS_STORED/2> sellStore{};
    
    void pushOrder(OrderMessage order){
        if(order.buyOrSell == BUY){
            if(buyNum < buyStore.size()){
                buyStore[buyNum++] = order;
                std::sort(buyStore.begin(), buyStore.begin() + buyNum
                , [](const OrderMessage a, const OrderMessage b){ return a.price > b.price;});
            }
        } else {
            if(sellNum < sellStore.size()){
                sellStore[sellNum++] = order;
                std::sort(sellStore.begin(), sellStore.begin() + sellNum
                , [](const OrderMessage a, const OrderMessage b){ return a.price < b.price;});
            }
        }
    }

    OrderMessage popBuyOrder(){
        return buyNum > 0 ? buyStore[--buyNum] : OrderMessage();
    }
    
    OrderMessage popSellOrder(){
        return sellNum > 0 ? sellStore[--sellNum] : OrderMessage();
    }
    
    OrderMessage showBuyOrder(){
        return buyNum > 0 ? buyStore[buyNum-1] : OrderMessage();
    }

    OrderMessage showSellOrder(){
        return sellNum > 0 ? sellStore[sellNum-1] : OrderMessage();
    }

    void changeBuyOrderAmount(int decrease){
        buyStore[buyNum-1].amountOfShares -= decrease;
    }

    void changeSellOrderAmount(int decrease){
        sellStore[sellNum-1].amountOfShares -= decrease;
    }
};

OrderMessage extractOrderFromFile(std::ifstream& orderFile);
void orderCheckAndExecute(OrderStore& orderStore);

int main(int argc, char* argv[]){
    
    auto start = std::chrono::steady_clock::now();
    std::ifstream orderFile(argv[1]);
    if(!orderFile){
        std::cerr << "Order File not read correctly \n";
        return 1;
    }
    OrderStore orderStorage{};

    bool endLoop = false;
    // Main execution loop
    while(!endLoop){

        // extraction and storage
        orderStorage.pushOrder(extractOrderFromFile(orderFile));

        // cancellation and execution
        orderCheckAndExecute(orderStorage);

        endLoop = orderFile.eof();
    }

    orderFile.close();

    auto stop = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Program run-time (seconds): " << std::setprecision(6) << duration.count()/1000000.0 << "s\n\n\n";
    return 0;
}

OrderMessage extractOrderFromFile(std::ifstream& orderFile){
    std::string tempStr{};
    std::array<std::string, 4> orderArr{};

    if(!getline(orderFile, tempStr, '\n')){
        return OrderMessage{ "", 0, 0, 0};
    }

    tempStr.erase(std::remove(tempStr.begin(), tempStr.end(), '\r'), tempStr.end());

    std::istringstream tokenStream(tempStr);
    std::string token{};
    int i = 0;
    while(getline(tokenStream, token, ',')){
        orderArr[i] = token;
        i++;
    }

    return OrderMessage{
        orderArr[0],
        std::stoi(orderArr[1]),
        std::stoi(orderArr[2]),
        std::stoi(orderArr[3])
    };

}



void orderCheckAndExecute(OrderStore& orderStore){
    // precedence given to orders that can be finished first and then to BUYs
    if(orderStore.buyNum <= 0|| orderStore.sellNum <= 0){
        std::cout << "empty stores \n";
        return;
    }
    while(orderStore.showBuyOrder().price <= orderStore.showSellOrder().price 
    && (orderStore.buyNum > 0 && orderStore.sellNum > 0)){
        int amountCarry = orderStore.showBuyOrder().amountOfShares - orderStore.showSellOrder().amountOfShares;
        if(amountCarry > 0){
            std::cout << std::format("Executed trade: {}, {}, {} cents, {} shares"
            , orderStore.showSellOrder().ticker, "SELL", orderStore.showSellOrder().price
            , orderStore.showSellOrder().amountOfShares) << "\n";
            orderStore.changeBuyOrderAmount(orderStore.showSellOrder().amountOfShares);
            orderStore.popSellOrder();

        } else {
            std::cout << std::format("Executed trade: {}, {}, {} cents, {} shares"
            , orderStore.showBuyOrder().ticker, "BUY", orderStore.showBuyOrder().price
            , orderStore.showBuyOrder().amountOfShares) << "\n";
            orderStore.changeSellOrderAmount(orderStore.showBuyOrder().amountOfShares);
            orderStore.popBuyOrder();
        }
    }
}



// TODO:
// Implement storage of buy sell orders
// Implement order execution logic
// Implement recieving of bids and asks and acknowledgements
// Implement order cancelation
// Implement per stock
// Make it FAST
