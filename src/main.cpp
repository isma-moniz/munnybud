#include "commands.hpp"

int Transaction::currentID = 1; // default value
std::string StorageHandler::default_wallet = "";

int main(int argc, char* argv[]) {
    if (argc>1) {
        return handleQuickInput(argc, argv);
    } else {
        return 0;
    }
}
