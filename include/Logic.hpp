#pragma once

#include "Common.hpp"
#include <fstream>
#include "OrderStore.hpp"

OrderMessage extractOrderFromFile(std::ifstream& orderFile);
void orderCheckAndExecute(OrderStore& orderStore);