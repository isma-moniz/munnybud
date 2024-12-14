#include "commands.hpp"
#include "interface.hpp"

int main(int argc, char* argv[]) {
    if (argc>1) {
        return handleQuickInput(argc, argv);
    } else {
        return runInterface();
    }
}