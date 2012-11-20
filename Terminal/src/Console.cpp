#include "AllegroTerminal/Terminal/Console.h"
#include "AllegroTerminal/Terminal/Commands.h"

#include <exception>

namespace etsai {
namespace allegroterminal {

using std::exception;
using std::vector;
using std::string;

Console::Console(int charPerLine, int maxLinesStored, int maxVisibleLines) : 
    charPerLine(charPerLine), maxVisibleLines(maxVisibleLines), begin(-1), 
    end(-1), visibleBegin(0), visibleEnd(0) {
    lines.resize(maxLinesStored);
}

vector<string> Console::getVisibleLines() const {
    vector<string> visibleLines(calcNumLines(visibleBegin, visibleEnd));
    bool terminate= (visibleBegin == visibleEnd);
    int vlIndex= 0, i= visibleBegin;
    
    while(!terminate) {
        visibleLines[vlIndex]= lines[i];
        increment(i);
        vlIndex++;
        terminate= (i > visibleEnd);
    }
    if (visibleBegin == visibleEnd) {
        visibleLines[0]= lines[visibleBegin];
    }
    return visibleLines;
}

int Console::getCharPerLine() const {
    return charPerLine;
}

int Console::getMaxVisibleLines() const {
    return maxVisibleLines;
}

void Console::addLine(const string& line) {
    size_t i= 0;

    while(i < line.size()) {
        string substr= line.substr(i, charPerLine);
        if (visibleEnd == end) {
            increment(visibleEnd);
        }
        increment(end);
        if (begin == end || begin == -1) {
            increment(begin);
        }
        lines[end]= substr;
    
        if (calcNumLines(visibleBegin, visibleEnd) > maxVisibleLines) {
            increment(visibleBegin);
        }

        i+= charPerLine;
    }
    
}

void Console::scrollUp(int numLines) {
    int diff= calcNumLines(begin, visibleBegin) - 1;
    int usedOffset= diff < numLines ? diff : numLines;
    
    decrement(visibleBegin, usedOffset);
    decrement(visibleEnd, usedOffset);
}

void Console::scrollDown(int numLines) {
    int diff= calcNumLines(visibleEnd, end) - 1;
    int usedOffset= diff < numLines ? diff : numLines;

    increment(visibleBegin, usedOffset);
    increment(visibleEnd, usedOffset);
}

int Console::calcNumLines(int left, int right) const {
    int max= lines.size();
    return (right - left + max) % max + 1;
}

void Console::increment(int &i) const {
    i= (i + 1) % lines.size();
}

void Console::increment(int &i, int offset) const {
    i= (i + offset) % lines.size();
}

void Console::decrement(int &i, int offset) const {
    i= (i - offset + lines.size()) % lines.size();
}

}
}
