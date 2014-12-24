/** @file   key_event.c
    @author Ran Bao (rba90) & Kane Williams (pkw21), Group1
    @date   30 September 2014
    @brief  Handles User Input
*/

#include "button.h"
#include "system.h"
#include "navswitch.h"
#include "key_event.h"

void key_event_init(void){
    navswitch_init();
    button_init();

    nav_north_pushed = 0;
    nav_south_pushed = 0;
    nav_west_pushed = 0;
    nav_east_pushed = 0;
    nav_pushed = 0;
    button_pushed = 0;
}


void key_event_p(void){
    navswitch_update();
    button_update();

    if (navswitch_push_event_p (NAVSWITCH_NORTH)){
        nav_north_pushed = 1;
    }
    if (navswitch_push_event_p (NAVSWITCH_SOUTH)){
        nav_south_pushed = 1;
    }
    if (navswitch_push_event_p (NAVSWITCH_WEST)){
        nav_west_pushed = 1;
    }
    if (navswitch_push_event_p (NAVSWITCH_EAST)){
        nav_east_pushed = 1;
    }
    if (navswitch_push_event_p(NAVSWITCH_PUSH)){
        nav_pushed = 1;
    }
    if (button_push_event_p(BUTTON1)){
        button_pushed = 1;
    }
}
