#include "commands.hpp"
#include "storage.hpp"
#include "utils.hpp"

int handleQuickInput(int argc, char* argv[]) {
    StorageHandler storageHandler;

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
    
    // 'add' subcommand
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
        .scan<'d', float>();

    add_cmd.add_argument("-l", "--label")
        .help("Brief description/label for the expense")
        .required();
    
    add_cmd.add_argument("-c", "--category")
        .help("Expense category")
        .default_value("default");

    add_cmd.add_argument("-d", "--date")
        .help("Date of the transaction")
        .default_value(getCurrentDate());

    // 'del' subcommand
    // TODO: implement deletion
    
    //view subcommand
    argparse::ArgumentParser view_cmd("view");
    view_cmd.add_argument("-d", "--date")
        .help("The base date of the transactions you want to consult")
        .required();
    
    view_cmd.add_argument("-r", "--range")
        .help("The range in days of the transactions to show. 1 will show the base day only, 2 will show the next also")
        .default_value(1)
        .scan<'i', int>();

    program.add_subparser(add_cmd);
    program.add_subparser(view_cmd);

    try {
        program.parse_args(argc, argv);
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return -1;
    }

    std::string operation = program.get<std::string>("operation");
    if (operation == "add") {
        float amount = add_cmd.get<float>("amount");
        std::string category = add_cmd.get<std::string>("--category");
        std::string label = add_cmd.get<std::string>("--label");
        std::string date = add_cmd.get<std::string>("--date");

        storageHandler.storeExpense(amount, category, label, date);
        std::cout << "Transaction stored!\n";
    } else if (operation == "view") {
        std::string date = view_cmd.get<std::string>("--date");
        int rng = view_cmd.get<int>("--range");

        if (storageHandler.retrieveExpensesByDate(date, rng) < 0) {
            std::cout << "Error when retrieving expenses.\n";
            return -1;
        }
    }
    return 0;
}