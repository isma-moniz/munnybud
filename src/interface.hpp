#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "transaction.hpp"


#include <vector>
#include <unordered_map>
#include <QApplication>
#include <QWidget>

void printResults(std::vector<Transaction>& results);
void printResultsGrouped(const std::string& groupBy, const std::unordered_map<std::string, std::vector<Transaction>>& groupedResults);
void printGroupedByCategory(const std::unordered_map<std::string, std::vector<Transaction>>& groupedResults);
void printGroupedByWallet(const std::unordered_map<std::string, std::vector<Transaction>>& groupedResults);
void printGroupedByDate(const std::unordered_map<std::string, std::vector<Transaction>>& groupedResults);

void drawWindow();

#endif
