#pragma once
#include "Common.hpp"
#include <array>

class OrderStore{
public:
    int buyNum{};
    int sellNum{};
    std::array<OrderMessage, MAX_NUMBER_OF_ORDERS_STORED/2> buyStore{};
    std::array<OrderMessage, MAX_NUMBER_OF_ORDERS_STORED/2> sellStore{};
    
    
    void pushOrder(OrderMessage order);
    OrderMessage popBuyOrder();
    
    OrderMessage popSellOrder();    
    OrderMessage showBuyOrder();
    OrderMessage showSellOrder();
    void changeBuyOrderAmount(int decrease);
    void changeSellOrderAmount(int decrease);

};