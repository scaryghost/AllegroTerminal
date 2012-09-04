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
    
    void addLine(const std::string &line);
    void scrollUp(int numLines);
    void scrollDown(int numLines);

private:
    int calcNumVisibleLines();

    int maxVisibleLines;
    int charPerLine;
    int begin;
    int end;
    std::vector<std::string> lines;

};  //class Console

}   //namespace allegroterminal
}   //namespace etsai

#endif  //ETSAI_ALLEGROTERMINAL_CONSOLE
