#include "AllegroTerminal/Commands.h"
#include "AllegroTerminal/Input.h"

namespace etsai {
namespace allegroterminal {

Input::Input() :
historyOffset(-1), inputPos(0) {
}

void Input::clear() {
    historyOffset= -1;
    inputPos= 0;
    oldInput.clear();
    current.clear();
}

bool Input::empty() const  {
    return current.empty();
}

unsigned int Input::size() const {
    return current.size();
}

bool Input::moveLeft(int offset) {
    bool canMove= inputPos != 0;

    inputPos-= offset;
    if (inputPos < 0) {
        inputPos= 0;
    }
    return canMove;
}
bool Input::moveRight(int offset) {
    bool canMove= inputPos < (int)current.size();

    inputPos+= offset;
    if (inputPos > (int)current.size()) {
        inputPos= current.size();
    }
    return canMove;
}

void Input::insertChar(char ch) {
    current.insert(inputPos, 1, ch);
    moveRight(1);
}
void Input::removeChar() {
    if (inputPos > 0) {
        current.erase(inputPos - 1, 1);
        moveLeft(1);
    }
}

bool Input::prevCommand() {
    if (historyOffset + 1 >= (int)Commands::getHistories().size()) {
        return false;
    }

    historyOffset++;
    if (historyOffset == 0) {
       oldInput= current;
    }
    current= Commands::getHistories()[historyOffset];
    inputPos= current.size();
    return true;
}
bool Input::nextCommand() {
    if (historyOffset == -1) {
        return false;
    }

    historyOffset--;
    if (historyOffset == -1) {
       current= oldInput;
    } else {
       current= Commands::getHistories()[historyOffset];
    }
    inputPos= current.size();
    return true;
}

const std::string& Input::getValue() const {
    return current;
}

}
}