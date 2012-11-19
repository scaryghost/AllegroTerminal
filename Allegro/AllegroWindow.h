#ifndef ETSAI_ALLEGROTERMINAL_ALLEGROWINDOW
#define ETSAI_ALLEGROTERMINAL_ALLEGROWINDOW

#include "AllegroTerminal/Terminal/terminal.h"

namespace etsai {
namespace allegroterminal {

class AllegroWindow : public Window {
public:
    AllegroWindow(ALLEGRO_FONT *font, Console *console);

    virtual void drawCursor();
    virtual void drawConsole();
    virtual void drawInput();

private:
    ALLEGRO_FONT *font;

};  //class AllegroWindow

}
}

#endif  //ETSAI_ALLEGROTERMINAL_ALLEGROWINDOW