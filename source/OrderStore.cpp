#include "OrderStore.hpp"
#include "Common.hpp"
#include <iostream>
#include <algorithm>

OrderStore::OrderStore(){
    std::make_heap(buyStore.begin(), buyStore.begin() + buyNum, [](OrderMessage& a, OrderMessage& b){return a.price < b.price;});
    std::make_heap(sellStore.begin(), sellStore.begin() + sellNum, [](OrderMessage& a, OrderMessage& b){return a.price > b.price;});
}
    
void OrderStore::pushOrder(OrderMessage order){
    if(order.buyOrSell == BUY){
        if(buyNum < buyStore.size()){
            buyStore[buyNum++] = order;
            std::push_heap(buyStore.begin(), buyStore.begin() + buyNum, [](OrderMessage& a, OrderMessage& b){return a.price < b.price;});
        } else {
            // will need to change to other methods of printing to console later when speed becomes an issue
            // std::cerr << "ALERT: Buy store full \n"; 
        }
    } else {
        if(sellNum < sellStore.size()){
            sellStore[sellNum++] = order;
            std::push_heap(sellStore.begin(), sellStore.begin() + sellNum, [](OrderMessage& a, OrderMessage& b){return a.price > b.price;});
        } else {
            // std::cerr << "ALERT: Sell store full \n";
        }
    }

}

OrderMessage OrderStore::popBuyOrder(){
    if(buyNum > 0){
        std::pop_heap(buyStore.begin(), buyStore.begin() + buyNum, [](OrderMessage& a, OrderMessage& b){return a.price < b.price;});
        return buyStore[--buyNum];
    }
    return OrderMessage();
}
    
OrderMessage OrderStore::popSellOrder(){
    if(sellNum > 0){
        std::pop_heap(sellStore.begin(), sellStore.begin() + sellNum, [](OrderMessage& a, OrderMessage& b){return a.price > b.price;});
        return sellStore[--sellNum];
    }
    return OrderMessage();
}
    
OrderMessage OrderStore::showBuyOrder(){
    return buyNum > 0 ? buyStore[0] : OrderMessage();
}

OrderMessage OrderStore::showSellOrder(){
    return sellNum > 0 ? sellStore[0] : OrderMessage();
}

// improve safety incase of negative values
void OrderStore::changeBuyOrderAmount(int decrease){
    buyStore[0].amountOfShares -= decrease;
    if(!buyStore[0].amountOfShares){
        popBuyOrder();
    }
}

void OrderStore::changeSellOrderAmount(int decrease){
    sellStore[0].amountOfShares -= decrease;
    if(!sellStore[0].amountOfShares){
        popSellOrder();
    }
}