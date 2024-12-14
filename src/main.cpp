#include "storage.hpp"

int main(void) {
    StorageHandler sHandler;
    json expenses;
    sHandler.retrieveExpensesByDate("2024-11-17", 2, expenses);
    std::cout << expenses << std::endl;
}
