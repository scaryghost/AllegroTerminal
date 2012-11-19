#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "AllegroTerminal/Terminal/terminal.h"
#include "AllegroTerminal/Allegro/AllegroWindow.h"

using namespace std;
using namespace etsai::allegroterminal;

void start();
void addCommands();

volatile bool endProgram= false, drawCursor= true;
int width, height;
Window *window;
Console *console;
ALLEGRO_DISPLAY *display= NULL;
ALLEGRO_EVENT_QUEUE *event_queue= NULL;
ALLEGRO_FONT *font= NULL;
ALLEGRO_TIMER *timer;
int fontW;

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
    al_init_primitives_addon();
    font= al_load_ttf_font(fontPath, atoi(argv[3]), 0);

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

    timer = al_create_timer(0.5);
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    fontW= al_get_text_width(font,"a");
    console= new Console(width/fontW, 4096, height/al_get_font_line_height(font) - 1);
    window= new AllegroWindow(font, console);

    cpl << "chars per line: " << console->getCharPerLine();
    mvl << "max visible lines: " << console->getMaxVisibleLines();
    console->addLine(cpl.str());
    console->addLine(mvl.str());

    addCommands();
    al_start_timer(timer);
    start();
    al_destroy_display(display);
    delete console;
    delete window;
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

    Commands::add("quit", [](const vector<string> &args) -> void {
        endProgram= true;
    });

    Commands::add("time", [](const vector<string> &args) -> void {
        time_t curr;
        const char* format= "%Y-%m-%d %H:%M:%S %Z";
        char timeStamp[80];

        time(&curr);
#ifdef WIN32
        tm timeInfo;
        localtime_s(&timeInfo, &curr);
        strftime(timeStamp, sizeof(timeStamp), format, &timeInfo);
#else
        tm* timeInfo;
        timeInfo= localtime(&curr);
        strftime(timeStamp, sizeof(timeStamp), format, timeInfo);
#endif
        console->addLine(string("Current time: ") + timeStamp);
    });

    Commands::add("history", [](const vector<string> &args) -> void {
        auto history= Commands::getHistories();
        int index= 0;

        for(auto it= history.end() - 1; it >= history.begin(); it--,index++) {
            stringstream line(stringstream::out);
            line << index << " " << *it;
            console->addLine(line.str());
        }
    });
}

void start() {
    vector<bool> pressed_keys(ALLEGRO_KEY_MAX, false);

    while(!endProgram) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        } else if(ev.type == ALLEGRO_EVENT_TIMER) {
            drawCursor= !drawCursor;
        } else if (ev.type == ALLEGRO_EVENT_KEY_CHAR) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
                window->removeChar();
            } else if (ev.keyboard.keycode == ALLEGRO_KEY_HOME) {
                window->moveToStart();
            } else if (ev.keyboard.keycode == ALLEGRO_KEY_END) {
                window->moveToEnd();
            } else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                window->execute();
            } else if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT) {
                window->moveCursorLeft(1);
            } else if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                window->moveCursorRight(1);
            } else if (ev.keyboard.keycode == ALLEGRO_KEY_UP) {
                window->prevCommand();
            } else if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                window->nextCommand();
            } else if ((ev.keyboard.modifiers & ALLEGRO_KEYMOD_CTRL) != ALLEGRO_KEYMOD_CTRL && ev.keyboard.unichar > 0) {
                window->addChar(char(ev.keyboard.unichar));
            }
        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            pressed_keys[ev.keyboard.keycode]= true;
            if (pressed_keys[ALLEGRO_KEY_LSHIFT] && pressed_keys[ALLEGRO_KEY_PGUP]) {
                window->scrollUp(4);
            } else if (pressed_keys[ALLEGRO_KEY_LSHIFT] && pressed_keys[ALLEGRO_KEY_PGDN]) {
                window->scrollDown(4);
            } else if (pressed_keys[ALLEGRO_KEY_LCTRL] && pressed_keys[ALLEGRO_KEY_A]) {
                window->moveToStart();
            } else if (pressed_keys[ALLEGRO_KEY_LCTRL] && pressed_keys[ALLEGRO_KEY_E]) {
                window->moveToEnd();
            } else if (pressed_keys[ALLEGRO_KEY_LCTRL] && pressed_keys[ALLEGRO_KEY_K]) {
                window->deletePastCursor();
            } else if (pressed_keys[ALLEGRO_KEY_LCTRL] && pressed_keys[ALLEGRO_KEY_U]) {
                window->deleteBeforeCursor();
            }
        } else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            pressed_keys[ev.keyboard.keycode]= false;
        }
        al_clear_to_color(al_map_rgb(0,0,0));
        window->drawConsole();
        window->drawInput();
        if (drawCursor) {
            window->drawCursor();
        }
        al_flip_display();
    }
}