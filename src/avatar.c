/** @file   avatar.c
    @author Ran Bao (rba90) & Kane Williams (pkw21), Group1
    @date   30 September 2014
    @brief  Creation and controlling of Avatar.
*/

#include "avatar.h"
#include "system.h"
#include "key_event.h"
#include "task.h"
#include "pio.h"
#include "misc.h"
#include "game.h" // get ball state
#include "communication.h" // player_seed
#include "key_event.h"

//Flashing Cursor
static uint8_t cursor_state = 0;

void cursor_flash(void){
    cursor_state = !cursor_state;
}

void init_avatar_creator(void){
    player_size = 0;
}

// Controls the cursor and positions blocks.
void create_avatar(void){
    static uint8_t px = 0, py = 0;

    if (DESTROYABLE_AVATAR){
        player_health = player_size;
    }
    else {
        player_health = PLAYER_HEALTH;
    }
    
    //Exit avatar-creation upon button-press.
    if (button_pushed){
        
        if (player_size != 0) {
            task_stop();
        }
    button_pushed = 0;
    }
    
    // Navigating screen with cursor.
    if (nav_pushed){

        // create a data pattern if the nav button is pushed
        // using the same method that is used in framebuffer
        uint8_t bitmask = BIT (py); //EG 6, -> 00100000
        uint8_t pattern = player[px] & ~bitmask; // EG 00100100 & 11011111

        if (player[px] & BIT(py)){
            // "Removes" Dot
            player_size -= 1;
        }
        else{
            // Add Dot
            if (player_size < MAXSIZE) {
				player_size += 1;
				pattern |= bitmask;
			}
        }
        
        player[px] = pattern;

        nav_pushed = 0;
    }
    else if (nav_north_pushed){
        // move in row direction, 0-6
        // Will loop the cursor around the screen
        py -= 1;
        py = mod(py, ROW_NUM);
        nav_north_pushed = 0;
    }
    else if (nav_south_pushed){
        // move in row direction, 0-6
        py += 1;
        py = mod(py, ROW_NUM);
        nav_south_pushed = 0;
    }
    else if (nav_west_pushed){
        // move in col direction, 0-4
        px -= 1;
        px = mod(px, COL_NUM);
        nav_west_pushed = 0;
    }
    else if (nav_east_pushed){
        // move in col direction, 0-4
        px += 1;
        px = mod(px, COL_NUM);
        nav_east_pushed = 0;
    }
    
    // clear the screen buffer
    fb_clear();

    // push the current avatar to screen buffer
    display_avatar();

    // directly push the cursor to the screen buffer
    fb_set_pixel(px, py, cursor_state);
}

void move_avatar(void){
    static uint8_t enable_shoot = 1;
    static uint16_t timeout = 0;
    timeout += 1;

    // shoot the ball every second
    if (timeout >= GAME_RATE * 1){
        timeout = 0;
        enable_shoot = 1;
    }
    
    // SHOOTING
    if (nav_pushed && enable_shoot){
        shoot_ball();
        enable_shoot = 0;
        nav_pushed = 0;
    }
    
    // NAVIGATION
    else if (nav_north_pushed){
        // shift all pattern in row direction for 1 bit
        for (uint8_t i = 0; i < COL_NUM; i++){
            // circular shift
            // eg.
            // 1100000 << 1 or 1100000 >> 6 -> 1000000 or 0000001 results in
            // 1000001

            player[i] = (player[i] >> 1) | (player[i] << (ROW_NUM - 1));
        }

        nav_north_pushed = 0;
    }
    else if (nav_south_pushed){
        // shift all pattern in row direction for 1 bit
        for (uint8_t i = 0; i < COL_NUM; i++){
            player[i] = (player[i] << 1) | (player[i] >> (ROW_NUM - 1));
        }

        nav_south_pushed = 0;
    }
    else if (nav_west_pushed){
        // shift all pattern in col direction for 1 bit
        // circular shift left the elements in player array
        uint8_t temp = player[0];
        for (uint8_t i = 0; i < COL_NUM - 1; i++)
        {
            player[i] = player[i + 1];
        }
        player[COL_NUM - 1] = temp;

        nav_west_pushed = 0;
    }
    else if (nav_east_pushed){
        // shift all pattern in col direction for 1 bit
        // circular shift right the elemtns in player array
        uint8_t temp = player[COL_NUM - 1];
        for (uint8_t i = COL_NUM - 1; i != 0; i--)
        {
            player[i] = player[i - 1];
        }
        player[0] = temp;

        nav_east_pushed = 0;
    }

    fb_clear();
    display_avatar();
}

void display_avatar(void){
    for (uint8_t i = 0; i < COL_NUM; i++){
        fb_set_col(i, player[i]);
    }
}

void shoot_ball(void){
    // Ball initialization Data

    // The bigger you are, the slower you shoot.
    int max_vel = (player_size < MAXSPEED) ? (MAXSPEED - player_size): 1;
    Vector init_p;

    Vector init_v = {-max_vel, 0};
    
    // scan for most right (player0) or most left (player1) bit
    // get the coordinate for x
    int8_t col;
    int8_t row;

    if (player_id == 0){
        for (col = 0; col < COL_NUM; col++){
            if (player[col]){
                init_p.x = (col - 1); // Shoot in front. (don't ask me why 0.5)
                break;
            }
        }
        // get the coordinate for y for highest bit
        for (row = 0; row < ROW_NUM; row++){
            if (player[col] & BIT(row)){
                init_p.y = (row);
            }
        }
    }

    else{
        for (col = 0; col < COL_NUM; col++){
            if (player[col]){
                init_p.x = (col - 1);
                break;
            }
        }
        // get the coordinate for y for lowest bit
        for (row = 0; row < ROW_NUM; row++){
            if (player[col] & BIT(row)){
                init_p.y = (row);
                break;
            }
        }
    }
    // shoot the ball
    ball_init(init_p, init_v);
}



const uint8_t zero[] = {0x3E, 0x51, 0x49, 0x45, 0x3E};
const uint8_t one[] = {0x00, 0x42, 0x7F, 0x40, 0x00};
void welcome_screen(void){
    
    if (player_id == 0){
        for (uint8_t i = 0; i < COL_NUM; i++){
            fb_set_col(i, zero[i]);
        }
    }
    else{
        for (uint8_t i = 0; i < COL_NUM; i++){
            fb_set_col(i, one[i]);
        }
    }

    if (nav_north_pushed){
        nav_north_pushed = 0;
        player_id = !player_id;
    }
    if (nav_south_pushed){
        nav_south_pushed = 0;
        player_id = !player_id;
    }
    if (button_pushed){
        button_pushed = 0;
        task_stop();
    }
}
