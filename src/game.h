/** @file   game.h
    @author Ran Bao (rba90) & Kane Williams (pkw21), Group1
    @date   7 October 2014
    @brief  game header file
*/

#ifndef GAME_H
#define GAME_H

#include "system.h"
#include "framebuffer.h"

// The ratio between GAME_RATE and STEP must be constant
// the motion looks naturally
#define STEP 0.008
#define GAME_RATE 300

#define X_BOUNDARY COL_NUM
#define Y_BOUNDARY ROW_NUM // If we had a health bar we change this.

#define INSIDE_WALL_BOUNCY 0 // Where IR is.
#define OUTSIDE_WALL_BOUNCY 1

#define ENABLE_BALL_COLLISIONS 1 // Balls bouncing with each other.


// Max: 2 with collision
// Max: 7 without collision
// If more, then CPU tick is not fast enough to handle.
#define MAX_NUM_BALLS 5

// Energy Loss = 1 is no loss. Energy Loss < 1 decreases energy.
#define GRAVITY 10
#define ENERGY_LOSS 1

// struct for Vector
typedef struct{
    float x;
    float y;
}Vector;

// new implementation of ball simulation
// 
// Ball struct has three properties
// v store the current position for ball
// p store the current velocity for ball
// state indicates the ball state, 0 for disable
// and 1 for enable
typedef struct {
    Vector v;
    Vector p;
    uint8_t state;
} Ball;

// ball_index counts the current number of balls on the screen
uint8_t ball_index;

// ball_list stores all balls
Ball ball_list[MAX_NUM_BALLS];

/**
 * Initilization of the ball and initial position and velocity
 * @param ball   Target ball
 * @param init_p initial position for ball
 * @param init_v initial velocity for ball
 */
void ball_init(Vector init_p, Vector init_v);

/**
 * Advance the simulation to next step
 * @param ball Target ball
 */
void ball_advance(Ball *ball);


/**
 * Disable the ball in the list
 * @param ball Target ball
 */
void ball_disable(Ball *ball);

/**
 * Calculate the actual collision of the ball
 * @param b1 Target ball
 * @param b2 Another ball
 */
void ball_collision_ball(Ball *b1, Ball *b2);
void ball_collision_avatar(Ball *b);

/**
 * Initilize the game, eg, set state of ball to 0
 */
void game_init(void);

/**
 * Iteriate through all balls to next step
 */
void game_advance(void);

/**
 * Initilize the tinygl with font3x5
 */
void game_end_init(void);

/**
 * End of game task that execute the ting_gl display and 
 * receive key task
 */
void game_end(void);

/**
 * Check the state of game
 * @return  0 for dead and 1 for not dead
 */
uint8_t check_death(void);

#endif
