#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <cstdlib>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "AllegroTerminal/Commands.h"
#include "AllegroTerminal/Console.h"

using namespace std;
using namespace etsai::allegroterminal;

void start();
void draw(const string& msg);
void addCommands();

Console *console;
const float FPS= 60;
int width, height;
ALLEGRO_DISPLAY *display= NULL;
ALLEGRO_EVENT_QUEUE *event_queue= NULL;
ALLEGRO_FONT *font= NULL;

int main(int argc, char **argv) {
    stringstream cpl(stringstream::out), mvl(stringstream::out);
    const char *fontPath= "DejaVuSansMono.ttf";
    width= atoi(argv[1]);
    height= atoi(argv[2]);

    if(!al_init()) {
        cerr << "failed to initialize allegro!\n";
        return -1;
    }

    al_init_font_addon();   // initialize the font addon
    al_init_ttf_addon();    // initialize the ttf (True Type Font) addon
    font= al_load_ttf_font(fontPath, 10, 0);

    event_queue = al_create_event_queue();
    if(!event_queue) {
        cerr << "failed to create event_queue!\n";
        return -1;
    }
    if(!al_install_keyboard()) {
        cerr << "failed to initialize the keyboard!\n";
        return -1;
    }

    display = al_create_display(width, height);
    if(!display) {
        cerr << "failed to create display!\n";
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    console= new Console(width/al_get_text_width(font,"a"), 4096, height/al_get_font_line_height(font));
    cpl << "chars per line: " << console->getCharPerLine();
    mvl << "max visible lines: " << console->getMaxVisibleLines();
    console->addLine(cpl.str());
    console->addLine(mvl.str());

    draw("");
    addCommands();
    start();
    al_destroy_display(display);
    delete console;
}

void addCommands() {
    Commands::add("add", [](const vector<string> &args) -> void {
        stringstream sum(stringstream::out);

        sum << atoi(args[0].c_str()) + atoi(args[1].c_str());
        console->addLine(sum.str());
    });

    Commands::add("echo", [](const vector<string> &args) -> void {
        console->addLine(args[0]);
    });
}

void start() {
    string msg;
    vector<string> visibleLines;

    while(true) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        } else if (ev.type == ALLEGRO_EVENT_KEY_CHAR) {
            if (ev.keyboard.keycode == 63) {
                msg= msg.substr(0, msg.length()-1);
            } else if (ev.keyboard.keycode == 67) {
                console->addLine(msg);
                try {
                    Commands::exec(msg);
                } catch (exception &ex) {
                    console->addLine(ex.what());
                }
                msg.clear();
            } else if (ev.keyboard.unichar > 0) {
                msg+= char(ev.keyboard.unichar);
            }

            draw(msg);
        }
    }
}

void draw(const string& msg) {
    vector<string> visibleLines= console->getVisibleLines();
    int y= 0;

    al_clear_to_color(al_map_rgb(0,0,0));
    al_draw_text(font, al_map_rgb(0,255,0), 0, height-al_get_font_line_height(font), ALLEGRO_ALIGN_LEFT, msg.c_str());
    for(auto it= visibleLines.begin(); it != visibleLines.end(); it++,y++) {
        al_draw_text(font, al_map_rgb(0,255,0), 0, y * al_get_font_line_height(font), ALLEGRO_ALIGN_LEFT, it->c_str());
    }
    al_flip_display();
}
