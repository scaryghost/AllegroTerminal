#include "AllegroTerminal/Commands.h"

#include <sstream>
#include <utility>

namespace etsai {
namespace allegroterminal {

using std::make_pair;
using std::runtime_error;
using std::string;
using std::stringstream;
using std::unordered_map;
using std::vector;

unordered_map<string, Commands::TermCommand> Commands::commands;
vector<string> Commands::history;

void Commands::add(std::string name, const TermCommand &comm) throw(std::runtime_error) {
    if (commands.count(name) != 0) {
        stringstream msg(stringstream::out);

        msg << "Command (" << name << ") already registered";
        throw runtime_error(msg.str().c_str());
    }
    commands.insert(make_pair(name, comm));
}

void Commands::exec(const std::string line) throw(std::runtime_error) {
    bool findName= true, quoteMode= false;
    string name, temp;
    vector<string> args;
    size_t i;

    history.insert(history.begin(), line);
    /** parse the command line */
    for(i= 0; i < line.size(); i++) {
        if (line[i] == '\"') {
            quoteMode= !quoteMode;
        } else if (line[i] != ' ' || quoteMode) {
            temp+= line[i];
        } else {
            if (findName) {
                name= temp;
                findName= false;
            } else {
                args.push_back(temp);
            }
            temp.clear();
        }
    }
    if (quoteMode) {
        throw runtime_error("Unmatched quotation \"");
    }
    if (!temp.empty()) {
        if (findName) {
            name= temp;
        } else {
            args.push_back(temp);
        }
    }
    if (commands.count(name) == 0) {
        stringstream msg(stringstream::out);
        
        msg << "Unrecognized command: " << name;
        throw runtime_error(msg.str().c_str());
    }

    commands[name](args);
}

const std::vector<std::string>& Commands::getHistories() {
    return history;
}

}   //namespace allegroterminal
}   //namespace etsai