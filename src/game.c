/** @file   game.c
    @author Ran Bao (rba90) & Kane Williams (pkw21), Group1
    @date   30 September 2014
    @brief  Controls the ball around the screen.
*/

#include "system.h"
#include "game.h"
#include "key_event.h"
#include "framebuffer.h"
#include "task.h"
#include "avatar.h" // get the player id
#include "misc.h"
#include "communication.h"
#include "tinygl.h"
#include "font3x5_1.h"
#include "misc.h"

void ball_init(Vector init_p, Vector init_v){
    // Loop around to oldest ball.
    ball_index = mod(ball_index+1, ball_index);
    
    Ball *ball = &ball_list[ball_index];
    ball->v = init_v;
    ball->p = init_p;
    ball->state = 1;
}

void ball_advance(Ball *ball){
    // Invert the gravity for player 0. (So screen is "flipped")
    if (player_id == 0){
        ball->v.y = ball->v.y + STEP * (-GRAVITY);
    }
    else{
        ball->v.y = ball->v.y + STEP * (GRAVITY);
    }
    ball->p.x = ball->p.x + STEP * ball->v.x;
    ball->p.y = ball->p.y + STEP * ball->v.y;

    // collide with ground
    if (ball->p.y <= 0 && ball->v.y < 0){
        ball->v.y = -ball->v.y * ENERGY_LOSS;
        ball->p.y = 0;
    }

    // collide with ceiling
    if (ball->p.y >= (Y_BOUNDARY - 1) && ball->v.y > 0){
        ball->v.y = -ball->v.y * ENERGY_LOSS;
        ball->p.y = Y_BOUNDARY - 1; 
    }
    

    // collide with inside wall
    if (ball->p.x <= 0 && ball->v.x < 0){
        if (!INSIDE_WALL_BOUNCY) {
            ir_send_ball(ball);
            ball_disable(ball);
        }
        else {
            ball->v.x = -ball->v.x * ENERGY_LOSS;
            ball->p.x = 0; 
        }

    // collide with outside wall
    }   
    if (ball->p.x >= (X_BOUNDARY - 1) && ball->v.x > 0){
        if (!OUTSIDE_WALL_BOUNCY){
            ball_disable(ball);
        }
        ball->v.x = -ball->v.x * ENERGY_LOSS;
        ball->p.x = X_BOUNDARY - 1; 
    }
}

void ball_disable(Ball *ball){
    ball->state = 0;
}

void ball_collision_avatar(Ball *b){
    
    uint8_t bitmask = BIT (rounds(b->p.y));
    uint8_t pattern = player[rounds(b->p.x)] & ~bitmask;

    // If intersection
    if (player[rounds(b->p.x)] & bitmask){
        b->state = 0;
        player_health -= 1;
        if (DESTROYABLE_AVATAR) {
            player[rounds(b->p.x)] = pattern;
            player_size -= 1;
        }
    }
}

void ball_collision_ball(Ball *b1, Ball *b2){
    // This is the simplified version of collision test, so don't
    // try to understand how it works. 
    // 
    // all balls have radius of 1
    // and mass of 1
    
    // 0.5 is the minimum distance between each ball
    if (distance(&b1->p, &b2->p) <= 0.5){
        // step 1 
        // find normal vector
        
        Vector un;
        un.x = b1->p.x - b2->p.x;
        un.y = b1->p.y - b2->p.y;

        // normalize it
        normalize(&un);

        // find the unit tangent vector
        Vector ut;
        ut.x = -un.y;
        ut.y = un.x;

        // find tangent and normal vector after collision
        float new_v1t, new_v2t;

        new_v1t = dot_product(&ut, &b1->v);
        new_v2t = dot_product(&ut, &b2->v);

        // find new normal velocities
        float new_v1n, new_v2n;

        new_v1n = dot_product(&un, &b2->v);
        new_v2n = dot_product(&un, &b1->v);

        // convert scalar normal and tengential veclcities into vectors
        
        b1->v.x = new_v1n * un.x + new_v1t * ut.x;
        b1->v.y = new_v1n * un.y + new_v1t * ut.y;

        b2->v.x = new_v2n * un.x + new_v2t * ut.x;
        b2->v.y = new_v2n * un.y + new_v2t * ut.y;

    }
}


void game_init(void){
    ball_index = 0;
    for (uint8_t i = 0; i < MAX_NUM_BALLS; i++){
        ball_list[i].state = 0;
    }
}
void game_advance(void){
    fb_clear();
    display_avatar();

    if (check_death() && !player_defeat){
        ir_send_defeat();
        player_defeat = 1;
        ir_send_stop();
    }

    for (uint8_t i = 0; i < MAX_NUM_BALLS; i++){
        if (ball_list[i].state){
            
            // Ball-ball realistic collisions
            if (ENABLE_BALL_COLLISIONS) {
                for (uint8_t j = i + 1; j < MAX_NUM_BALLS; j++){
                    if (ball_list[j].state){
                        ball_collision_ball(&ball_list[i], &ball_list[j]);
                    }
                }
            }
            
            ball_collision_avatar(&ball_list[i]);
            
            ball_advance(&ball_list[i]);
            
            // Display Ball
            fb_set_pixel(rounds(ball_list[i].p.x), rounds(ball_list[i].p.y), 1);
        }
        
    }



}

void game_end_init(void){
    tinygl_init(DISPLAY_TASK_RATE);
    tinygl_font_set (&font3x5_1);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
    tinygl_clear();
    if (player_defeat){
        tinygl_text("LOSE");
    }
    else{
        tinygl_text("WIN");
    }
}

void game_end(void){
    if (button_pushed){
        task_stop();
        button_pushed = 0;
    }
}

uint8_t check_death(void){
    if (player_health <= 0 && !INVINCIBLE){
        return 1;
    }
    return 0;
}
