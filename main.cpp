#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "AllegroTerminal/Commands.h"
#include "AllegroTerminal/Console.h"

using namespace std;
using namespace etsai::allegroterminal;

void start();
void draw(const string& msg, int offset, int cursorPos);
void addCommands();

volatile bool endProgram= false, drawCursor= true;
int width, height;
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
    cpl << "chars per line: " << console->getCharPerLine();
    mvl << "max visible lines: " << console->getMaxVisibleLines();
    console->addLine(cpl.str());
    console->addLine(mvl.str());

    draw("", 0, 0);
    addCommands();
    al_start_timer(timer);
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
    int cursorPos= 0;
    size_t charNo= 0;
    int offset= 0;
    int historyOffset= -1;
    vector<string> visibleLines;
    vector<bool> pressed_keys(ALLEGRO_KEY_MAX, false);
    string input, oldInput;

    while(!endProgram) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        } else if(ev.type == ALLEGRO_EVENT_TIMER) {
            drawCursor= !drawCursor;
        } else if (ev.type == ALLEGRO_EVENT_KEY_CHAR) {
            if (ev.keyboard.keycode == 63) {
                if (charNo != 0) {
                    cursorPos--;
                    charNo--;
                    input.erase(charNo, 1);
                }
            } else if (ev.keyboard.keycode == 67) {
                if (!input.empty()) {
                    try {
                        console->addLine("> " + input);
                        Commands::exec(input);
                    } catch (exception &ex) {
                        console->addLine(ex.what());
                    }
                    charNo= 0;
                    cursorPos= 0;
                    offset= 0;
                    historyOffset= -1;
                    input.clear();
                }
            } else if (ev.keyboard.keycode == 82) {
                charNo--;
                cursorPos--;
            } else if (ev.keyboard.keycode == 83 && charNo < input.size()) {
                charNo++;
                cursorPos++;
            } else if (ev.keyboard.keycode == 85 && historyOffset > -1) {
                historyOffset--;
                if (historyOffset == -1) {
                    input= oldInput;
                } else {
                    input= Commands::getHistories()[historyOffset];
                }
                charNo= input.size();
                cursorPos= input.size() % console->getCharPerLine();
            } else if (ev.keyboard.keycode == 84 && historyOffset < (int)Commands::getHistories().size() - 1) {
                historyOffset++;
                if (historyOffset == 0) {
                    oldInput= input;
                }
                input= Commands::getHistories()[historyOffset];
                charNo= input.size();
                cursorPos= input.size() % console->getCharPerLine();
            } else if (ev.keyboard.unichar > 0) {
                input.insert(charNo, 1, char(ev.keyboard.unichar));
                charNo++;
                cursorPos++;
            }
        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            pressed_keys[ev.keyboard.keycode]= true;
            if (pressed_keys[ALLEGRO_KEY_LSHIFT] && pressed_keys[ALLEGRO_KEY_PGUP]) {
                console->scrollUp(4);
            } else if (pressed_keys[ALLEGRO_KEY_LSHIFT] && pressed_keys[ALLEGRO_KEY_PGDN]) {
                console->scrollDown(4);
            }
        } else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            pressed_keys[ev.keyboard.keycode]= false;
        }
        if (cursorPos < 0) {
            offset--;
            cursorPos= 0;
        } else if (cursorPos > console->getCharPerLine()) {
            offset++;
            cursorPos= console->getCharPerLine();
        }
        draw(input, offset, cursorPos);
    }
}

void draw(const string& msg, int offset, int cursorPos) {
    vector<string> visibleLines= console->getVisibleLines();
    int y= 0;

    al_clear_to_color(al_map_rgb(0,0,0));
    if (drawCursor) {
        al_draw_text(font, al_map_rgb(0,255,0), cursorPos * fontW, height-al_get_font_line_height(font), ALLEGRO_ALIGN_CENTRE, "|");
    }
    al_draw_text(font, al_map_rgb(0,255,0), 0, height-al_get_font_line_height(font), ALLEGRO_ALIGN_LEFT, msg.substr(offset, console->getCharPerLine()).c_str());
    for(auto it= visibleLines.begin(); it != visibleLines.end(); it++,y++) {
        al_draw_text(font, al_map_rgb(0,255,0), 0, y * al_get_font_line_height(font), ALLEGRO_ALIGN_LEFT, it->c_str());
    }
    al_flip_display();
}
