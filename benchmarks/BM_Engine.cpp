#include <benchmark/benchmark.h>
#include <fstream>
#include <string>
#include "OrderStore.hpp"
#include "Logic.hpp"
#include "RandomOrder.hpp"
#include <iostream>

std::string g_filename = "";

static void BM_OrderExtraction_10k(benchmark::State& states){
    std::ifstream orderFile(g_filename);
    
    OrderStore store;

    for(auto _ : states){
        orderFile.clear();
        orderFile.seekg(0);

        OrderMessage var;
        for(int i = 0; i < 10'000; i++){
            var = extractOrderFromFile(orderFile);
            benchmark::DoNotOptimize(var);
        }

    }
    states.SetItemsProcessed(states.iterations() * 10'000);
    orderFile.close();
}

BENCHMARK(BM_OrderExtraction_10k);

static void BM_OrderStorage_10k(benchmark::State& states){
    OrderStore store{};

    // load orders to store
    std::vector<OrderMessage> tempOrders{};
    for(int i = 0; i < 10'000; i++){
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
    states.SetItemsProcessed(states.iterations() * 10'000);
}

BENCHMARK(BM_OrderStorage_10k);

// doesnt actually match 10k orders but goes through 10k store
static void BM_OrderMatching_10k(benchmark::State& states){
    OrderStore* mstore = new OrderStore();
    OrderStore store;

    std::vector<OrderMessage> tempOrders{};
    for(int i = 0; i < 10'000; i++){
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

BENCHMARK(BM_OrderMatching_10k);

int main(int argc, char** argv) {
    benchmark::Initialize(&argc, argv);

    if (argc > 1) {
        g_filename = argv[1];
    }

    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
    return 0;
}