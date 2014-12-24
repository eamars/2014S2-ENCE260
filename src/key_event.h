/** @file   key_event.h
    @author Ran Bao (rba90) & Kane Williams (pkw21), Group1
    @date   7 October 2014
    @brief  key press header file
*/

#ifndef KEY_EVENT_H
#define KEY_EVENT_H

#include "system.h"

#define BUTTON_TASK_RATE 50



uint8_t nav_north_pushed;
uint8_t nav_south_pushed;
uint8_t nav_west_pushed;
uint8_t nav_east_pushed; 
uint8_t nav_pushed;
uint8_t button_pushed;

/**
 * initilize all nav keys and button
 */
void key_event_init(void);

/**
 * use scheduler to take the press event of nav keys and button
 */
void key_event_p(void);


#endif