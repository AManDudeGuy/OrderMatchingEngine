#include <iostream>
#include <string>
#include <fstream>
#include "Common.hpp"
#include "OrderStore.hpp"
#include "Logic.hpp"

// Very Naive order matching engine that works only for a single ticker

int main(int argc, char* argv[]){
    
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
    return 0;
}