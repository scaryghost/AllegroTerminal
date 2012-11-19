#include "AllegroTerminal/Terminal/Window.h"
#include "AllegroTerminal/Terminal/Commands.h"

#include <algorithm>
#include <exception>

namespace etsai {
namespace allegroterminal {

using std::exception;
using std::max;
using std::min;

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
                if (inputOffset < 4) {
                    cursorPos= inputOffset;
                    inputOffset= 0;
                } else {
                    inputOffset-= 4;
                    cursorPos= 4;
                }
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
    int end= console->getCharPerLine() - 1;

    if (charPos + offset > input.size()) {
        actualOffset= input.size() - charPos;
        charPos= input.size();
        cursorPos= min((int)input.size(), end);
        inputOffset+= actualOffset;
    } else {
        actualOffset= offset;
        cursorPos+= offset;
        charPos+= offset;
    }
    if (cursorPos > end) {
        inputOffset+= cursorPos - end;
        cursorPos= end;
    }
    return actualOffset;
}

void Window::moveToStart() {
    moveCursorLeft(input.size());
}

void Window::moveToEnd() {
    moveCursorRight(input.size());
}

void Window::scrollUp(int offset) {
    console->scrollUp(offset);
}

void Window::scrollDown(int offset) {
    console->scrollDown(offset);
}

void Window::addChar(char ch) {
    input.insert(charPos, 1, ch);
    moveCursorRight(1);
}

void Window::removeChar() {
    if (charPos > 0) {
        input.erase(charPos - 1, 1);
        moveCursorLeft(1);
    }
}

void Window::deletePastCursor() {
    input.erase(charPos);
}

void Window::deleteBeforeCursor() {
    input.erase(0, charPos);
    cursorPos= 0;
    inputOffset= 0;
    charPos= 0;
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
        cursorPos= min((int)input.size(), console->getCharPerLine());
        inputOffset= max((int)input.size() - console->getCharPerLine(), 0);
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
        cursorPos= min((int)input.size(), console->getCharPerLine());
        inputOffset= max((int)input.size() - console->getCharPerLine(), 0);
        charPos= input.size();
    }
}

}
}