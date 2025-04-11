#include "commands.hpp"
#include "interface.hpp"

int Transaction::currentID = 1; // default value
std::string StorageHandler::default_wallet = "";

int main(int argc, char* argv[]) {
    if (argc>1) {
        return handleQuickInput(argc, argv);
    } else {
        QApplication app(argc, argv);
        QWidget window;
        window.setWindowTitle("My First Qt5 Window");
        window.resize(800, 600);  // Resize the window to 800x600
        window.show();  // Show the window

        // Start the event loop
        return app.exec();
        return 0;
    }
}
