#ifndef ETSAI_ALLEGROTERMINAL_COMMANDS
#define ETSAI_ALLEGROTERMINAL_COMMANDS

#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace etsai {
namespace allegroterminal {

/**
 * Stores the set of commands the terminal can execute
 * @author etsai
  */
class Commands {
public:
    typedef std::function<void (const std::vector<std::string> &args)> TermCommand;

    static void add(std::string name, const TermCommand &comm) throw(std::runtime_error);
    static void exec(const std::string line) throw(std::runtime_error);

private:
    static std::unordered_map<std::string, TermCommand> commands;
};

}   //namespace allegroterminal
}   //namespace etsai

#endif      //ETSAI_ALLEGROTERMINAL_COMMANDS