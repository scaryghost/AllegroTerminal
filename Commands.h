#ifndef ETSAI_ALLEGROTERMINAL_COMMANDS
#define ETSAI_ALLEGROTERMINAL_COMMANDS

#include <functional>
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

    static void add(const TermCommand &comm);
    static void exec(const std::vector<std::string> argv);

private:
    static std::unordered_map<std::string, TermCommand> commands;
};

}   //namespace allegroterminal
}   //namespace etsai

#endif      //ETSAI_ALLEGROTERMINAL_COMMANDS