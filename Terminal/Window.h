#ifndef ETSAI_ALLEGROTERMINAL_WINDOW
#define ETSAI_ALLEGROTERMINAL_WINDOW

#include "AllegroTerminal/Terminal/Console.h"
#include <string>

namespace etsai {
namespace allegroterminal {

class Window {
public:
    Window(Console* console);

    int moveCursorLeft(int offset);
    int moveCursorRight(int offset);
    void scrollUp(int offset);
    void scrollDown(int offset);

    void addChar(char ch);
    void removeChar();
    void execute();
    void prevCommand();
    void nextCommand();
    
    virtual void drawCursor()= 0;
    virtual void drawConsole()= 0;
    virtual void drawInput()= 0;

protected:
    Console* console;
    unsigned int cursorPos;
    unsigned int charPos;
    unsigned int inputOffset;
    int historyOffset;
    std::string input, savedInput;
};  //class Window

}   //namespace allegroterminal
}   //namespace etsai

#endif