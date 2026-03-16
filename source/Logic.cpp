#include "Logic.hpp"
#include "Common.hpp"
#include <sstream>
#include <format>
#include <string>
#include <iostream>

OrderMessage extractOrderFromFile(std::ifstream& orderFile){
    std::string tempStr{};
    std::array<std::string, 4> orderArr{};

    if(!getline(orderFile, tempStr, '\n')){
        return OrderMessage{ "", 0, 0, 0};
    }

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
    static int ordersExecuted{};
    // precedence given to orders that can be finished first and then to BUYs
    if(orderStore.buyNum <= 0|| orderStore.sellNum <= 0){
        return;
    }
    while(orderStore.showBuyOrder().price >= orderStore.showSellOrder().price 
    && (orderStore.buyNum > 0 && orderStore.sellNum > 0)){
        int amountCarry = orderStore.showBuyOrder().amountOfShares - orderStore.showSellOrder().amountOfShares;
        // std::cout << std::format("Executed trade: BuyOrder: {}, {}, {} cents, {} shares \t SellOrder: {}, {}, {} cents, {} shares"
        // , orderStore.showBuyOrder().ticker, "BUY", orderStore.showBuyOrder().price, orderStore.showBuyOrder().amountOfShares
        // , orderStore.showSellOrder().ticker, "SELL", orderStore.showSellOrder().price, orderStore.showSellOrder().amountOfShares)
        // << " Orders Executed: " << ++ordersExecuted << "\n";
        if(amountCarry > 0){
            orderStore.changeBuyOrderAmount(orderStore.showSellOrder().amountOfShares);
            orderStore.popSellOrder();
        } else {
            orderStore.changeSellOrderAmount(orderStore.showBuyOrder().amountOfShares);
            orderStore.popBuyOrder();
        }
    }
}