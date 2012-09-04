#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <cstdlib>
#include <iostream>
#include <sstream>

using namespace std;

void start();

const float FPS= 60;
int width, height;
ALLEGRO_DISPLAY *display= NULL;
ALLEGRO_EVENT_QUEUE *event_queue= NULL;
ALLEGRO_FONT *font= NULL;

int main(int argc, char **argv) {
    width= atoi(argv[1]);
    height= atoi(argv[2]);

    if(!al_init()) {
        cerr << "failed to initialize allegro!\n";
        return -1;
    }
    al_init_font_addon();   // initialize the font addon
    al_init_ttf_addon();    // initialize the ttf (True Type Font) addon
    font= al_load_ttf_font("pirulen.ttf",10,0 );

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
    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();

    start();
    al_destroy_display(display);
}

void start() {
    while(true) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            stringstream msg(stringstream::out);

            msg << ev.keyboard.keycode;
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_text(font, al_map_rgb(0,255,0), width/2, (height/4),ALLEGRO_ALIGN_CENTRE, msg.str().c_str());
            al_flip_display();
        }
    }
}
