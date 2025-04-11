#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "argparse.hpp"
#include "storage.hpp"

int handleQuickInput(int argc, char* argv[]);
void setupAddCmd(argparse::ArgumentParser& add_cmd);
void setupViewCmd(argparse::ArgumentParser& view_cmd);
int handleSetupCmd();
int handleAddCmd(argparse::ArgumentParser& add_cmd, StorageHandler& storageHandler);
int handleViewCmd(argparse::ArgumentParser& view_cmd, StorageHandler& storageHandler);
#endif
