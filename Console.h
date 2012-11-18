#ifndef ETSAI_ALLEGROTERMINAL_CONSOLE
#define ETSAI_ALLEGROTERMINAL_CONSOLE

#include <vector>
#include <string>

namespace etsai {
namespace allegroterminal {

class Console {
public:
    Console(int charPerLine, int maxLinesStored, int maxVisibleLines);
    ~Console();

    std::vector<std::string> getVisibleLines() const;
    int getCharPerLine() const;
    int getMaxVisibleLines() const;
    
    void addLine(const std::string &line);
    void scrollUp(int numLines);
    void scrollDown(int numLines);

protected:
    int calcNumLines(int left, int right) const;
    void increment(int &i) const;
    void increment(int &i, int offset) const;
    void decrement(int &i, int offset) const;

private:
    int charPerLine;
    int maxVisibleLines;
    int begin;
    int end;
    int visibleBegin;
    int visibleEnd;
    std::string input;
    std::vector<std::string> lines;
};  //class Console

}   //namespace allegroterminal
}   //namespace etsai

#endif  //ETSAI_ALLEGROTERMINAL_CONSOLE
