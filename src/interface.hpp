#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "transaction.hpp"
#include "storage.hpp"
#include <ncurses.h>

#include <vector>
#include <unordered_map>

class Interface {
private:
    // quick command mode
    void printGroupedByCategory(const std::unordered_map<std::string, std::vector<Transaction>>& groupedResults);
    void printGroupedByWallet(const std::unordered_map<std::string, std::vector<Transaction>>& groupedResults);
    void printGroupedByDate(const std::unordered_map<std::string, std::vector<Transaction>>& groupedResults);

    // curses mode 
    int width, height;
    WINDOW* mainWindow;

    void redraw(WINDOW* win);
    void drawBoxWStr(const std::string& str, int row, int col);
    void drawTransactionBox(const Transaction& transaction);
    WINDOW* createNewWin(int height, int width, int starty, int startx);
    void destroyWin(WINDOW* local_win);
public:
    // quick command mode
    void printResults(std::vector<Transaction>& results);
    void printResultsGrouped(const std::string& groupBy, const std::unordered_map<std::string, std::vector<Transaction>>& groupedResults); 
    // curses mode
    void initInterface(StorageHandler& storageHandler);
};

#endif
