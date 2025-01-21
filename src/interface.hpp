#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "transaction.hpp"

#include "json.hpp"
#include <iostream>
#include <vector>
#include <unordered_map>

int runInterface();

void printResults(const std::vector<Transaction>& results, const std::string& filter, const std::string& groupBy);
void printGroupedByCategory(const std::unordered_map<std::string, std::vector<Transaction>>& groupedResults);
void printGroupedByWallet(const std::unordered_map<std::string, std::vector<Transaction>>& groupedResults);
void printGroupedByDate(const std::unordered_map<std::string, std::vector<Transaction>>& groupedResults);

#endif
