#include "commands.hpp"
#include "utils.hpp"
#include "interface.hpp"
#include <ostream>

void setupAddCmd(argparse::ArgumentParser& add_cmd) {
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
        .scan<'g', float>();

    add_cmd.add_argument("-l", "--label")
        .help("Brief description/label for the expense")
        .required();

    add_cmd.add_argument("-c", "--category")
        .help("Expense category")
        .default_value(std::string("default"));

    add_cmd.add_argument("-d", "--date")
        .help("Date of the transaction")
        .default_value(getCurrentDate());

    add_cmd.add_argument("-w", "--wallet")
        .help("Wallet to charge expense from")
        .default_value(std::string("default"));
    return;
}

void setupViewCmd(argparse::ArgumentParser& view_cmd) {
    view_cmd.add_argument("-d", "--date")
        .help("The base date of the transactions you want to consult. Default is today.")
        .default_value(std::string("")); // this will get replaced with the current date if no other filters are used

    view_cmd.add_argument("-r", "--range")
        .help("The range in days of the transactions to show. 1 will show the base day only, 2 will show week, 3 will show month")
        .default_value(1)
        .scan<'i', int>();

    view_cmd.add_argument("-c", "--category")
        .help("Use this to filter results by a certain category.")
        .default_value(std::string(""));

    view_cmd.add_argument("-w", "--wallet")
        .help("Use this to filter results by a certain wallet")
        .default_value(std::string(""));

    view_cmd.add_argument("-g", "--group")
        .help("Use this to group the results. They are grouped by date by default, but you can also group by category or wallet")
        .default_value(std::string("date"))
        .action([](const std::string& groupBy) {
                if (groupBy != "date" && groupBy != "category" && groupBy != "wallet") {
                    throw std::invalid_argument("Invalid groupBy: must be 'date', 'wallet' or 'category'");
                }
                return groupBy;
        });

    return;
}

int handleSetupCmd() {
    std::cout << "Seting up wallets..." << std::endl;
    if (StorageHandler::setupWallets("../wallets.json") != 0)
        return -1;
    std::cout << "Done!" << std::endl;
    std::cout << "Setting up transaction file..." << std::endl;
    if (StorageHandler::setupTransactions("../transactions.json") != 0)
        return -1;
    std::cout << "Done!" << std::endl;
    return 0;
}

int handleAddCmd(argparse::ArgumentParser& add_cmd, StorageHandler& storageHandler) {
    std::string transaction = add_cmd.get<std::string>("transaction");
    float amountFloat = add_cmd.get<float>("amount");
    int amount = static_cast<int>(std::round(amountFloat * 100));
    std::string category = add_cmd.get<std::string>("--category");
    std::string label = add_cmd.get<std::string>("--label");
    std::string date = add_cmd.get<std::string>("--date");
    std::string wallet = add_cmd.get<std::string>("--wallet");
    if (transaction == "expense") {
        Transaction tx(-amount, category, label, wallet);
        tx.date = date;
        if (storageHandler.storeTransaction(tx) < 0)
            return -1;
    } else {
        Transaction tx(amount, category, label, wallet);
        tx.date = date;
        if (storageHandler.storeTransaction(tx) < 0)
            return -1;
    }

    std::cout << "Amount: " << amount << std::endl;
    std::cout << "Transaction stored!\n";
    return 0;
}

int handleViewCmd(argparse::ArgumentParser& view_cmd, StorageHandler& storageHandler) {
    std::string date = view_cmd.get<std::string>("--date");
    int rng = view_cmd.get<int>("--range");
    std::string wallet = view_cmd.get<std::string>("--wallet");
    std::string category = view_cmd.get<std::string>("--category");
    std::string groupBy = view_cmd.get<std::string>("--group");
    std::unordered_map<std::string, std::vector<Transaction>> result;

    if (storageHandler.retrieveTransactions(date, rng, wallet, category, result, groupBy) < 0)   {
        std::cout << "No expenses made in specified range.\n";
        return -1;
    }

    printResultsGrouped(groupBy, result);
    return 0;
}

int handleQuickInput(int argc, char* argv[]) {
    // program root command
    argparse::ArgumentParser program("munnybud");

    // 'setup' subcommand
    argparse::ArgumentParser stp_cmd("setup");

    // 'add' subcommand
    argparse::ArgumentParser add_cmd("add");
    setupAddCmd(add_cmd);
    
    // 'delete' subcommand
    argparse::ArgumentParser del_cmd("delete");
    del_cmd.add_argument("id")
        .help("ID of the transaction to delete")
        .scan<'i', int>();

    // 'view' subcommand
    argparse::ArgumentParser view_cmd("view");
    setupViewCmd(view_cmd);
    
    // 'balance' subcommand
    argparse::ArgumentParser balance_cmd("balance");
    balance_cmd.add_argument("-w", "--wallet")
        .help("The wallet you want to consult")
        .default_value(std::string("default"));
    
    // add subparsers to program root
    program.add_subparser(balance_cmd);
    program.add_subparser(add_cmd);
    program.add_subparser(view_cmd);
    program.add_subparser(del_cmd);
    program.add_subparser(stp_cmd);
    
    // parse arguments
    try {
        program.parse_args(argc, argv);
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return -1;
    }
    
    // handle 'setup' subcommand
    if (program.is_subcommand_used("setup")) {
        return handleSetupCmd(); 
    }
    
    // other subcommands require a storageHandler to be constructed!
    StorageHandler storageHandler("../wallets.json", "../transactions.json");

    // handle 'add' subcommand
    if (program.is_subcommand_used("add")) {
        return handleAddCmd(add_cmd, storageHandler);

    // handle 'view' subcommand
    } else if (program.is_subcommand_used("view")) {
        return handleViewCmd(view_cmd, storageHandler);
    
    // handle 'balance' subcommand
    } else if (program.is_subcommand_used("balance")) {
        std::string wallet = balance_cmd.get<std::string>("--wallet");
        float balance = storageHandler.retrieveBalance(wallet);
        std::cout << "Your current balance: " << balance << std::endl;

    // handle 'delete' subcommand
    } else if (program.is_subcommand_used("delete")) {
        int id = del_cmd.get<int>("id");
        if (storageHandler.deleteTransaction(id) < 0) {
            return -1;
        }
    }
    return 0;
}
