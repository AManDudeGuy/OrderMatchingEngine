#include "OrderStore.hpp"
#include "Common.hpp"
#include <algorithm>

    
void OrderStore::pushOrder(OrderMessage order){
    if(order.buyOrSell == BUY){
        if(buyNum < buyStore.size()){
            buyStore[buyNum++] = order;
            std::sort(buyStore.begin(), buyStore.begin() + buyNum
            , [](const OrderMessage a, const OrderMessage b){ return a.price < b.price;});
        }
    } else {
        if(sellNum < sellStore.size()){
            sellStore[sellNum++] = order;
            std::sort(sellStore.begin(), sellStore.begin() + sellNum
            , [](const OrderMessage a, const OrderMessage b){ return a.price > b.price;});
        }
    }
}

OrderMessage OrderStore::popBuyOrder(){
    return buyNum > 0 ? buyStore[--buyNum] : OrderMessage();
}
    
OrderMessage OrderStore::popSellOrder(){
    return sellNum > 0 ? sellStore[--sellNum] : OrderMessage();
}
    
OrderMessage OrderStore::showBuyOrder(){
    return buyNum > 0 ? buyStore[buyNum-1] : OrderMessage();
}

OrderMessage OrderStore::showSellOrder(){
    return sellNum > 0 ? sellStore[sellNum-1] : OrderMessage();
}

void OrderStore::changeBuyOrderAmount(int decrease){
    buyStore[buyNum-1].amountOfShares -= decrease;
    if(!buyStore[buyNum-1].amountOfShares){
        popBuyOrder();
    }
}

void OrderStore::changeSellOrderAmount(int decrease){
    sellStore[sellNum-1].amountOfShares -= decrease;
    if(!sellStore[sellNum-1].amountOfShares){
        popSellOrder();
    }
}