#include "commands.hpp"
#include "interface.hpp"
#include <curses.h>

Interface interface;
int Transaction::currentID = 1; // default value
std::string StorageHandler::default_wallet = "";

int main(int argc, char* argv[]) {
    if (argc>1) {
        return handleQuickInput(argc, argv, interface);
    } else {
        StorageHandler storageHandler("../wallets.json", "../transactions.json");
        interface.initInterface(storageHandler); 
        return 0;
    }
}
