#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "transaction.hpp"
#include "storage.hpp"
#include <ncurses.h>

#include <vector>
#include <unordered_map>

void printResults(std::vector<Transaction>& results);
void printResultsGrouped(const std::string& groupBy, const std::unordered_map<std::string, std::vector<Transaction>>& groupedResults);
void printGroupedByCategory(const std::unordered_map<std::string, std::vector<Transaction>>& groupedResults);
void printGroupedByWallet(const std::unordered_map<std::string, std::vector<Transaction>>& groupedResults);
void printGroupedByDate(const std::unordered_map<std::string, std::vector<Transaction>>& groupedResults);

void initInterface(StorageHandler& storageHandler);
void drawBoxWStr(const std::string& str, int row, int col);
WINDOW* createNewWin(int height, int width, int starty, int startx);
void destroyWin(WINDOW* local_win);

#endif
