#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "transaction.hpp"

#include "json.hpp"
#include <iostream>
#include <vector>
#include <unordered_map>

int runInterface();

void printResults(const std::vector<Transaction>& results, const std::string& filter="date");

#endif
