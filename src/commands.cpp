#include "commands.hpp"
#include "utils.hpp"

int handleQuickInput(int argc, char* argv[]) {
    argparse::ArgumentParser program("munnybud");

    // main command
    program.add_argument("operation")
        .help("The operation you want to perform: add, del or view")
        .action([](const std::string& op){
            if (op != "add" && op != "del" && op != "view") {
                throw std::runtime_error("Invalid operation: must be 'add', 'del' or 'view'");
            }
            return op;
        });
    
    // subcommands
    argparse::ArgumentParser add_cmd("add");
    add_cmd.add_argument("transaction")
        .help("Whether it's an expense or an income")
        .action([](const std::string& op) {
            if (op != "expense" && op != "income") {
                throw std::runtime_error("Invalid transaction: must be 'expense' or 'income'");
            }
            return op;
        });
    
    add_cmd.add_argument("amount")
        .help("Amount of money earned or spent")
        .scan<'d', double>();
    
    add_cmd.add_argument("--category")
        .help("Expense category")
        .default_value("default");

    add_cmd.add_argument("--date")
        .help("Date of the transaction")
        .default_value(getCurrentDate());

    try {
        program.parse_args(argc, argv);
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }
}