#include "AllegroTerminal/Allegro/AllegroWindow.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#include <string>
#include <vector>

namespace etsai {
namespace allegroterminal {

using std::string;
using std::vector;

AllegroWindow::AllegroWindow(ALLEGRO_FONT *font, Console *console) : Window(console) {
    this->font= font;
}

void AllegroWindow::drawCursor() {
    int fontW= al_get_text_width(font,"a");
    int yPos= console->getMaxVisibleLines() * al_get_font_line_height(font);
    al_draw_filled_rectangle(charPos * fontW, yPos, (charPos + 1) * fontW, yPos, al_map_rgb(0,255,0));
    al_draw_text(font, al_map_rgb(0,0,0), 0, yPos, ALLEGRO_ALIGN_LEFT, input.substr(charPos, 1).c_str());
}

void AllegroWindow::drawConsole() {
    vector<string> visibleLines= console->getVisibleLines();
    int y= 0;

    for(auto it= visibleLines.begin(); it != visibleLines.end(); it++,y++) {
        al_draw_text(font, al_map_rgb(0,255,0), 0, y * al_get_font_line_height(font), ALLEGRO_ALIGN_LEFT, it->c_str());
    }
}

void AllegroWindow::drawInput() {
    al_draw_text(font, al_map_rgb(0,255,0), 0, console->getMaxVisibleLines() * al_get_font_line_height(font), ALLEGRO_ALIGN_LEFT, input.substr(inputOffset, console->getCharPerLine()).c_str());
}

}
}
