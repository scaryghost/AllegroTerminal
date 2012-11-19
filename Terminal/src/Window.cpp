#include "AllegroTerminal/Terminal/Window.h"
#include "AllegroTerminal/Terminal/Commands.h"

#include <exception>

namespace etsai {
namespace allegroterminal {

using std::exception;

Window::Window(Console* console) :
cursorPos(0), charPos(0), inputOffset(0), historyOffset(-1) {
    this->console= console;
}

int Window::moveCursorLeft(int offset) {
    int actualOffset;

    if (charPos < offset) {
        actualOffset= charPos;
        charPos= 0;
        cursorPos= 0;
        inputOffset= 0;
    } else {
        while(offset > 0) {
            if(cursorPos == 0) {
                cursorPos= 4;
                inputOffset-= 4;
            }
            cursorPos--;
            offset--;
            charPos--;
        }
        actualOffset= offset;
    }
    return actualOffset;
}

int Window::moveCursorRight(int offset) {
    int actualOffset;

    if (charPos + offset > input.size()) {
        actualOffset= input.size() - charPos;
        charPos= input.size();
        cursorPos= input.size();
        inputOffset+= actualOffset;
    } else {
        actualOffset= offset;
        cursorPos+= offset;
        charPos+= offset;
        if (cursorPos > console->getCharPerLine()) {
            inputOffset+= console->getCharPerLine() - cursorPos;
            cursorPos= console->getCharPerLine();
        }
    }
    return actualOffset;
}

void Window::scrollUp(int offset) {
    console->scrollUp(offset);
}

void Window::scrollDown(int offset) {
    console->scrollDown(offset);
}

void Window::addChar(char ch) {
    input.insert(charPos, 1, ch);
    charPos++;
    moveCursorRight(1);
}

void Window::removeChar() {
    if (charPos > 0) {
        input.erase(charPos - 1);
        moveCursorLeft(1);
    }
}

void Window::execute() {
    if (!input.empty()) {
        try {
           console->addLine("> " + input);
           Commands::exec(input);
        } catch (exception &ex) {
           console->addLine(ex.what());
        }
        cursorPos= 0;
        inputOffset= 0;
        charPos= 0;
        historyOffset= -1;
        input.clear();
    }
}

void Window::prevCommand() {
    if (historyOffset + 1 < (int)Commands::getHistories().size()) {
        historyOffset++;
        if (historyOffset == 0) {
            savedInput= input;
        }
        input= Commands::getHistories()[historyOffset];
        charPos= input.size();
    }
}

void Window::nextCommand() {
    if (historyOffset != -1) {
        historyOffset--;
        if (historyOffset == -1) {
            input= savedInput;
        } else {
            input= Commands::getHistories()[historyOffset];
        }
        charPos= input.size();
    }
}

}
}