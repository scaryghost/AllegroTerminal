#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "AllegroTerminal/Terminal/terminal.h"
#include "AllegroTerminal/Allegro/AllegroWindow.h"

using namespace std;
using namespace etsai::allegroterminal;

void start(ALLEGRO_EVENT_QUEUE *event_queue);
void addCommands();

volatile bool endProgram= false;
Window *window;
Console *console;

int main(int argc, char **argv) {
    stringstream cpl(stringstream::out), mvl(stringstream::out);
    const char *fontPath= "DejaVuSansMono.ttf";
    int width= atoi(argv[1]), height= atoi(argv[2]);

    if(!al_init()) {
        cerr << "failed to initialize allegro!\n";
        return -1;
    }

    al_init_font_addon();   // initialize the font addon
    al_init_ttf_addon();    // initialize the ttf (True Type Font) addon
    al_init_primitives_addon();
    ALLEGRO_FONT *font= al_load_ttf_font(fontPath, atoi(argv[3]), 0);

    ALLEGRO_EVENT_QUEUE *event_queue= al_create_event_queue();
    if(!event_queue) {
        cerr << "failed to create event_queue!\n";
        return -1;
    }
    if(!al_install_keyboard()) {
        cerr << "failed to initialize the keyboard!\n";
        return -1;
    }

    ALLEGRO_DISPLAY *display= al_create_display(width, height);
    if(!display) {
        cerr << "failed to create display!\n";
        return -1;
    }

    ALLEGRO_TIMER *timer= al_create_timer(0.5);
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    console= new Console(width/al_get_text_width(font,"a"), 4096, height/al_get_font_line_height(font) - 1);
    window= new AllegroWindow(font, console);

    cpl << "chars per line: " << console->getCharPerLine();
    mvl << "max visible lines: " << console->getMaxVisibleLines();
    console->addLine(cpl.str());
    console->addLine(mvl.str());

    addCommands();
    al_start_timer(timer);
    start(event_queue);
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

void start(ALLEGRO_EVENT_QUEUE *event_queue) {
    bool drawCursor= true;
    unordered_map<int, function<void ()> > keybinds, ctrlKeyBinds, shiftKeyBinds;

    keybinds[ALLEGRO_KEY_BACKSPACE]= []() -> void {
        window->removeChar();
    };
    keybinds[ALLEGRO_KEY_HOME]= []() -> void {
        window->moveToStart();
    };
    keybinds[ALLEGRO_KEY_END]= []() -> void {
        window->moveToStart();
    };
    keybinds[ALLEGRO_KEY_ENTER]= []() -> void {
        window->execute();
    };
    keybinds[ALLEGRO_KEY_LEFT]= []() -> void {
        window->moveCursorLeft(1);
    };
    keybinds[ALLEGRO_KEY_RIGHT]= []() -> void {
        window->moveCursorRight(1);
    };
    keybinds[ALLEGRO_KEY_UP]= []() -> void {
        window->prevCommand();
    };
    keybinds[ALLEGRO_KEY_DOWN]= []() -> void {
        window->nextCommand();
    };

    ctrlKeyBinds[ALLEGRO_KEY_A]= []() -> void {
        window->moveToStart();
    };
    ctrlKeyBinds[ALLEGRO_KEY_E]= []() -> void {
        window->moveToEnd();
    };
    ctrlKeyBinds[ALLEGRO_KEY_K]= []() -> void {
        window->deletePastCursor();
    };
    ctrlKeyBinds[ALLEGRO_KEY_U]= []() -> void {
        window->deleteBeforeCursor();
    };
    
    shiftKeyBinds[ALLEGRO_KEY_PGUP]= []() -> void {
        window->scrollUp(4);
    };
    shiftKeyBinds[ALLEGRO_KEY_PGDN]= []() -> void {
        window->scrollDown(4);
    };
    while(!endProgram) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        switch(ev.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                endProgram= true;
                break;
            case ALLEGRO_EVENT_TIMER:
                drawCursor= !drawCursor;
                break;
            case ALLEGRO_EVENT_KEY_CHAR:
                if ((ev.keyboard.modifiers & ALLEGRO_KEYMOD_CTRL) ==  ALLEGRO_KEYMOD_CTRL) {
                    ctrlKeyBinds[ev.keyboard.keycode]();
                } else {
                    if (keybinds.count(ev.keyboard.keycode) != 0) {
                    keybinds[ev.keyboard.keycode]();
                    } else {
                        if ((ev.keyboard.modifiers & ALLEGRO_KEYMOD_SHIFT) ==  ALLEGRO_KEYMOD_SHIFT && shiftKeyBinds.count(ev.keyboard.keycode) != 0) {
                            shiftKeyBinds[ev.keyboard.keycode]();
                        } else {
                            window->addChar(char(ev.keyboard.unichar));
                        }
                    }
                }
                break;
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