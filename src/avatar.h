/** @file   avatar.h
    @author Ran Bao (rba90) & Kane Williams (pkw21), Group1
    @date   7 October 2014
    @brief  avatar header file
*/

#ifndef AVATAR_H
#define AVATAR_H

#include "system.h"
#include "framebuffer.h"

#define CURSOR_RATE 3 // the flash frequency of cursor

#define MAXSPEED 6 // Speed of ball     
#define MAXSIZE 7
#define PLAYER_HEALTH 3 // When DESTROYABLE_AVATAR is 0 this takes effect.

#define DESTROYABLE_AVATAR 1 // Can take bits and pieces off enemy avatar
#define INVINCIBLE 0

/**
 * Avatar is represented as a array of bit pattern.
 * eg, player1[COL_NUM] = 
 * {0000000,
 *  0011000,
 *  0001000,
 *  0000000,
 *  0000000}
 *  It makes the player easy to move around.
 */
uint8_t player[COL_NUM];
uint8_t player_id;
int8_t player_health;
uint8_t player_size;
uint8_t player_defeat;

// Assigns variables
void init_avatar_creator(void);

/**
 * Create a new avatar using nav_keys
 */
void create_avatar(void);

/**
 * move the avatar when playing games
 */
void move_avatar(void);

/**
 * Support function for rendering avatar
 */
void display_avatar(void);

/**
 * Support function for changing state of cursor
 */
void cursor_flash(void);

void shoot_ball(void);


void welcome_screen(void);

#endif
