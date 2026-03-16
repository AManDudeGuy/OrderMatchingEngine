#include <benchmark/benchmark.h>
#include <fstream>
#include <string>
#include "OrderStore.hpp"
#include "Logic.hpp"
#include "RandomOrder.hpp"
#include <iostream>

std::string g_filename = "";

static void BM_OrderExtraction(benchmark::State& states){
    std::ifstream orderFile(g_filename);
    
    OrderStore store;

    for(auto _ : states){
        orderFile.clear();
        orderFile.seekg(0);

        OrderMessage var;
        for(int i = 0; i < 100'000; i++){
            var = extractOrderFromFile(orderFile);
            benchmark::DoNotOptimize(var);
        }

    }

    orderFile.close();
}

BENCHMARK(BM_OrderExtraction);

static void BM_OrderStorage_100k(benchmark::State& states){
    OrderStore store{};

    // load orders to store
    std::vector<OrderMessage> tempOrders{};
    for(int i = 0; i < 10000; i++){
        tempOrders.push_back(randomOrderMessage({"AAPL"}));
    }

    for(auto _ : states){

        for(const auto& om : tempOrders){
            store.pushOrder(om);
        }

        benchmark::ClobberMemory();
        states.PauseTiming();
        store.buyNum = 0;
        states.ResumeTiming();
    }
    states.SetItemsProcessed(states.iterations() * 10000);
}

BENCHMARK(BM_OrderStorage_100k);


static void BM_OrderMatching(benchmark::State& states){
    OrderStore* mstore = new OrderStore();
    OrderStore store;

    std::vector<OrderMessage> tempOrders{};
    for(int i = 0; i < 10000; i++){
        mstore->pushOrder(randomOrderMessage({"AAPL"}));
    }

    for(auto _ : states){

        states.PauseTiming();

        // store orders to execute
        store = *mstore;

        states.ResumeTiming();

        orderCheckAndExecute(store);

        benchmark::ClobberMemory();
    }

}

BENCHMARK(BM_OrderMatching);

int main(int argc, char** argv) {
    benchmark::Initialize(&argc, argv);

    if (argc > 1) {
        g_filename = argv[1];
    }

    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
    return 0;
}