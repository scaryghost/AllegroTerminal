#include "AllegroTerminal/Console.h"

namespace etsai {
namespace allegroterminal {

using std::vector;
using std::string;

Console::Console(int charPerLine, int maxLinesStored, int maxVisibleLines) : 
    charPerLine(charPerLine), lines(maxLinesStored),
    maxVisibleLines(maxVisibleLines), begin(0), end(0) {
}

Console::~Console() {
}

vector<string> Console::getVisibleLines() const {
    vector<string> visibleLines(calcNumVisibleLines());
    bool terminate= (begin == end);
    int vlIndex= 0;
    
    while(!terminate) {
        visibleLines[vlIndex]= lines[i];
        i= (i + 1) % lines.size();
        vlIndex++;
        terminate= (i <= end);
    }
    return visibleLines;
}

void Console::addLine(const string& line) {
    end= (end + 1) % lines.size();
    lines[end]= line;
    
    if (calcNumVisibleLines() > maxVisibleLines)
        begin= (begin + 1) % lines.size();
}

void Console::scrollUp(int numLines) {
}

void Console::scrollDown(int numLines) {
}

int Console::calcNumVisibleLines() {
    return (end - start + lines.size()) % lines.size() + 1;
}

}
}
