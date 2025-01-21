#include "commands.hpp"

int Transaction::currentID = 1; // default value

int main(int argc, char* argv[]) {
    if (argc>1) {
        return handleQuickInput(argc, argv);
    } else {
        return 0;
    }
}
