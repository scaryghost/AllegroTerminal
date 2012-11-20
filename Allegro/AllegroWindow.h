#ifndef ETSAI_ALLEGROTERMINAL_ALLEGROWINDOW
#define ETSAI_ALLEGROTERMINAL_ALLEGROWINDOW

#include "AllegroTerminal/Terminal/terminal.h"
#include <allegro5/allegro_font.h>

namespace etsai {
namespace allegroterminal {

 /**
  * Allegro based implementation of the Window abstract class
  * @author etsai
  */
class AllegroWindow : public Window {
public:
    /**
     * Create an Allegro window using the specified font and console
     * @param   font    Font to use
     * @param   console Console to create the window with
     */
    AllegroWindow(ALLEGRO_FONT *font, Console *console);

    virtual void drawCursor();
    virtual void drawConsole();
    virtual void drawInput();

private:
    ALLEGRO_FONT *font;     ///< Font to display text in the terminal

};  //class AllegroWindow

}
}

#endif  //ETSAI_ALLEGROTERMINAL_ALLEGROWINDOW