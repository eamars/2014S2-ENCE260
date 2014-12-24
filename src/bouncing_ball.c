/** @file   bouncing_ball.c
    @author Ran Bao (rba90) & Kane Williams (pkw21), Group1
    @date   7 October 2014
    @brief  Main File. Runs the task schedulers.
*/

#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "display.h"
#include "task.h"
#include "ir_uart.h"
#include "framebuffer.h"
#include "key_event.h"
#include "game.h"
#include <stdlib.h>
#include "avatar.h"
#include "led.h"
#include "random.h"
#include "communication.h"
#include "font3x5_1.h"


int main(void){

    // Initialisation
    system_init();
    fb_init();
    led_init();
    random_init();
    key_event_init();
    ir_uart_init();
    player_init();

l1:
//----------------------------------------------------------------------
    // Task scheduler for determining the player id
    led_set(LED1, 0);
    

		task_t tasks0[] = {
			{.func = key_event_p, .period = TASK_RATE / BUTTON_TASK_RATE},
			{.func = ir_send_queue_daemon, .period = TASK_RATE / IR_RATE},
			{.func = fb_flush, .period = TASK_RATE / DISPLAY_TASK_RATE},

			{.func = player_recv_seed, .period = TASK_RATE / IR_RATE},
			{.func = player_send_seed, .period = TASK_RATE / IR_RATE}
        };

        task_schedule (tasks0, ARRAY_SIZE (tasks0));
    
    task_t tasks1[] = {
        {.func = fb_flush, .period = TASK_RATE / DISPLAY_TASK_RATE},
        {.func = key_event_p, .period = TASK_RATE / BUTTON_TASK_RATE},
        
        {.func = welcome_screen, .period = TASK_RATE / GAME_RATE}
    };

    task_schedule (tasks1, ARRAY_SIZE (tasks1));
    
    
//----------------------------------------------------------------------
    // Intial Task Scheduler. Deals with creating avatar.
    init_avatar_creator();
    
    task_t tasks2[] = {
        {.func = fb_flush, .period = TASK_RATE / DISPLAY_TASK_RATE},
        {.func = key_event_p, .period = TASK_RATE / BUTTON_TASK_RATE},
        
        {.func = create_avatar, .period = TASK_RATE / GAME_RATE},
        {.func = cursor_flash, .period = TASK_RATE / CURSOR_RATE}
    };

    task_schedule (tasks2, ARRAY_SIZE (tasks2));

//----------------------------------------------------------------------
    // Game Task Scheduler. Deals with moving the ball around screen.
    game_init();
    
    task_t tasks3[] = {
        {.func = fb_flush, .period = TASK_RATE / DISPLAY_TASK_RATE},
        {.func = key_event_p, .period = TASK_RATE / BUTTON_TASK_RATE},
        
        {.func = ir_send_queue_daemon, .period = TASK_RATE / IR_RATE},
        {.func = ir_recv_ball, .period = TASK_RATE / IR_RATE},

        {.func = move_avatar, .period = TASK_RATE / GAME_RATE},
        {.func = game_advance, .period = TASK_RATE / GAME_RATE}
    };

    task_schedule (tasks3, ARRAY_SIZE (tasks3));
    
    
//----------------------------------------------------------------------
    // End-Game message.
    game_end_init();

    task_t tasks4[] = {
        {.func = tinygl_update, .period = TASK_RATE / DISPLAY_TASK_RATE},
        {.func = key_event_p, .period = TASK_RATE / BUTTON_TASK_RATE},
        {.func = game_end, .period = TASK_RATE / GAME_RATE}
    };

    task_schedule (tasks4, ARRAY_SIZE (tasks4));
    
goto l1;
    return 0;
}
