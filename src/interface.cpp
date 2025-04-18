#include "interface.hpp"
#include "utils.hpp"
#include <cctype>
#include <stdexcept>
#include <iostream>

void printResults(std::vector<Transaction>& results) {
    for (const auto& transaction : results) {
        std::cout << "Date: " << transaction.date << '\n';
        std::cout << "ID: " << transaction.id << '\n';
        std::cout << "Amount: " << std::fixed << std::setprecision(2) << transaction.amount / 100.0 << '\n';
        std::cout << "Category: " << transaction.category << '\n';
        std::cout << "Description: " << transaction.description << '\n';
        std::cout << "Wallet: " << transaction.wallet << '\n';
        std::cout << '\n';
    }
}

void printResultsGrouped(const std::string& groupBy, const std::unordered_map<std::string, std::vector<Transaction>>& groupedResults) {
    if (groupBy == "date") {
        printGroupedByDate(groupedResults);
    } else if (groupBy == "category") {
        printGroupedByCategory(groupedResults);
    } else if (groupBy == "wallet") {
        printGroupedByWallet(groupedResults);
    } else {
        throw std::runtime_error("Invalid grouping category: " + groupBy + "\n");
    }
}

void printGroupedByDate(const std::unordered_map<std::string, std::vector<Transaction>>& groupedResults) {
    std::cout << "Expenses grouped by date: " << std::endl << std::endl;
    for (const auto& [key, transactions] : groupedResults) {
        std::cout << "Date: " << key << std::endl << std::endl;
        for (const auto& transaction : transactions) {
            std::cout << "ID: " << transaction.id << std::endl;
            std::cout << "Amount: " << std::fixed << std::setprecision(2) << transaction.amount / 100.0 << std::endl;
            std::cout << "Category: " << transaction.category << std::endl;
            std::cout << "Description: " << transaction.description << std::endl;
            std::cout << "Wallet: " << transaction.wallet << std::endl;
            std::cout << std::endl;
        }
    }
}

void printGroupedByCategory(const std::unordered_map<std::string, std::vector<Transaction>>& groupedResults) {
    std::cout << "Expenses grouped by category: " << std::endl;
    for (const auto& [key, transactions] : groupedResults) {
        std::cout << "Category: " << key << std::endl << std::endl;
        for (const auto& transaction : transactions) {
            std::cout << "ID: " << transaction.id << std::endl;
            std::cout << "Date: " << transaction.date << std::endl;
            std::cout << "Amount: " << std::fixed << std::setprecision(2) << transaction.amount / 100.0 << std::endl;
            std::cout << "Description: " << transaction.description << std::endl;
            std::cout << "Wallet: " << transaction.wallet << std::endl;
            std::cout << std::endl;
        }
    }
}

void printGroupedByWallet(const std::unordered_map<std::string, std::vector<Transaction>>& groupedResults) {
    std::cout << "Expenses grouped by wallet: " << std::endl;
    for (const auto& [key, transactions] : groupedResults) {
        std::cout << "Wallet: " << key << std::endl << std::endl;
        for (const auto& transaction : transactions) {
            std::cout << "ID: " << transaction.id << std::endl;
            std::cout << "Date: " << transaction.date << std::endl;
            std::cout << "Amount: " << std::fixed << std::setprecision(2) << transaction.amount / 100.0 << std::endl;
            std::cout << "Description: " << transaction.description << std::endl;
            std::cout << "Category: " << transaction.category << std::endl;
            std::cout << std::endl;
        }
    }
}

void initInterface(StorageHandler& storageHandler) {
    WINDOW* transactionsWin;
    int startx, starty, width, height;
    int ch;

    initscr();
    cbreak();

    keypad(stdscr, TRUE);

    height = 100;
    width = 100;
    starty = 2;
    startx = 2;
    printw("Press F1 to exit");
    refresh();
    transactionsWin = createNewWin(height, width, starty, startx);
    std::unordered_map<std::string, std::vector<Transaction>> transactionMap;
    storageHandler.retrieveTransactions("2025-01-17", 3, "", "", transactionMap, "date");
    std::vector<Transaction> transactionLog = storageHandler.getResultsGrouped(transactionMap);
    for (Transaction& transaction : transactionLog) {
        mvwprintw(transactionsWin, ++starty, startx, "%d", transaction.amount); 
    }

    wrefresh(transactionsWin);
    getch();

    endwin();
}

void drawBoxWStr(const std::string& str, int row, int col) {
    // Top border
    move(row, col);
    attron(COLOR_PAIR(1));
    addch(ACS_ULCORNER);
    for (int i = 0; i < str.length() + 2; i++) {
        addch(ACS_HLINE);
    }
    addch(ACS_URCORNER);

    // Middle row with text
    move(row + 1, col);
    addch(ACS_VLINE);
    printw(" %s ", str.c_str());
    addch(ACS_VLINE);

    // Bottom border
    move(row + 2, col);
    addch(ACS_LLCORNER);
    for (int i = 0; i < str.length() + 2; i++) {
        addch(ACS_HLINE);
    }    
    addch(ACS_LRCORNER);
    attroff(COLOR_PAIR(1));
}

WINDOW* createNewWin(int height, int width, int starty, int startx) {
    WINDOW* local_win;

    local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0); //two provided integers are the chartype of the border
    wrefresh(local_win);
    return local_win;
}

void destroyWin(WINDOW* local_win) {
    wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '); // ugly ass function lmao
    wrefresh(local_win);
    delwin(local_win);
}
