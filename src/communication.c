/** @file   communication.c
    @author Ran Bao (rba90) & Kane Williams (pkw21), Group1
    @date   30 September 2014
    @brief  Communicates between multiple UCFK4s.
*/

#include "communication.h"
#include "system.h"
#include "ir_uart.h"
#include "random.h"
#include "avatar.h"
#include "game.h"
#include "task.h"
#include "led.h"
#include "framebuffer.h"
#include "misc.h"
#include "key_event.h"


/**
 * Queue implementation of ir tasks
 * With the certain size of queue, the qfront and qrear is used to 
 * indicates the location of the current message.
 * 
 * Queue is a 16 bit signed array with queue_size, which is default set
 * to 10 to avoid using too much memory. 
 *
 * Queue is initialized to -1 to indicates the current item is empty
 * that the task will stop sending message until the next buffer receieved.
 *
 * It is suggested that the ir_send() should have higher frequency than 
 * IR_RATE so that the ir could send contineous message. OR use
 * ir_send_queue_pause() to pause the task until ir_send_queue_continue() is
 * called.
 */
#define QUEUE_SIZE 6
#define EMPTY_SYM -1

static int16_t queue[QUEUE_SIZE] = {[0 ... QUEUE_SIZE - 1] = EMPTY_SYM}; // this method only works with GCC
static uint8_t qfront = 0, qrear = 0;
static uint8_t ir_pause = 0;
static uint8_t ir_stop = 0;

void ir_send_queue_daemon(void){
    // have something in queue
    if (queue[qfront] != EMPTY_SYM && !ir_pause){
        led_set(LED1, 1);
        ir_uart_putc ((uint8_t)queue[qfront]);
        queue[qfront++] = EMPTY_SYM;
        qfront %= QUEUE_SIZE;
        led_set(LED1, 0);
    }
    // exit the task only if the queue is empty
    else if (ir_stop){
        task_stop();
        ir_stop = 0;
    }

    

}

void ir_send_queue_pause(void){
    ir_pause = 1;
}

void ir_send_queue_continue(void){
    ir_pause = 0;
}

void ir_send_stop(void){
    // set stop flag to true, which the task will end immediately 
    // one the queue is empty
    ir_stop = 1;
}

void ir_send(uint8_t msg){
    // push the message to queue
    queue[qrear++] = msg;
    qrear %= QUEUE_SIZE;
}


void player_init(void){
    // generate the random seed from 1 to 250
    // The message greater than 250 is control command
    player_seed = randi(1, 250);
}

void player_recv_seed(void){
    // receive the incoming message
    if (ir_uart_read_ready_p ()){
        IR_BUFFER_RECV = ir_uart_getc ();
    }

    // if receive any message that is not zero or not idle
    if (IR_BUFFER_RECV != 0x00 && IR_BUFFER_RECV != IDLE){
        // terminate the task iff the player_seed not collide
        if (player_seed != IR_BUFFER_RECV){
            // we agree on that if the player_seed < other_seed, then
            // we are player 0, otherwise we are player1
            if (player_seed < IR_BUFFER_RECV){
                player_id = 0;
            }
            else{
                player_id = 1;
            }

            // response back to other
            ir_send(player_seed);

            // terminate the task
            ir_send_stop();
        }
        IR_BUFFER_RECV = IDLE;
    }
    
}

// some awsome animation when waiting for trigging the player_seed sync.
uint8_t comm_sym[] = {0x10, 0x20, 0x7c, 0x20, 0x10};
void player_send_seed(void){
    static uint8_t tick = 0;
    tick += 1;

    // advance the pattern every 100ms
    if (tick >= IR_RATE * 0.1){
        tick = 0;
    }

    fb_clear();
    for (uint8_t i = 0; i < COL_NUM; i++){
        // advance the pattern
        if (tick == 0){
            comm_sym[i] = (comm_sym[i] << 1) | (comm_sym[i] >> (ROW_NUM - 1));
        }
        fb_set_col(i, comm_sym[i]);
    }

    // send player_id if button is pressed
    if (button_pushed){
        ir_send(player_seed);
        button_pushed = 0;
    }
}




void ir_send_ball(Ball *ball){
    /**
     * Use fp2bin to convert float number to unsigned integer
     * see misc.c for more detail
     */
    ir_send_queue_pause();
    ir_send(fp2bin(ball->p.y + 1)); // magic trick, dont ask me why
    ir_send(fp2bin(ball->v.x));
    ir_send(fp2bin(ball->v.y));
    ir_send_queue_continue();
}

void ir_recv_ball(void){
    static uint8_t stage = 0;
    static uint8_t timeout = 0;
    static Vector init_p, init_v;
    static uint8_t win = 0;
    
    // Set 500 ms timeout so that if the ball failed to transmit, eg,
    // some packet was lost during the transmission. if timeout then we 
    // can throw away the packet we have already received and start off 
    // again


    timeout += 1;
    if (timeout >= IR_RATE * 0.5){
        timeout = 0;
        stage = 0;
    }

    
    if (ir_uart_read_ready_p ()){
        IR_BUFFER_RECV = ir_uart_getc ();
    }

    if (IR_BUFFER_RECV != IDLE){
        led_set(LED1, 1);
        // We want to receive three data, which are p.y, v.x and v.y.
        // At each stage, if nothing received, then return to stage 0 
        // if timeout. If we have already received three data, then 
        // initialize a new ball and add to ball_list
        if (stage == 0){
            // keep the x coordinate same but reverse the y coordinate
            init_p.y = Y_BOUNDARY - bin2fp(IR_BUFFER_RECV);
            init_p.x = 0;
            stage += 1;

            if (IR_BUFFER_RECV == DEFEAT){
                win += 1;
            }
        }
        else if (stage == 1){
            // reverse the velocity at x direction
            // since player0 and player1 have different direction of gravity
            init_v.x = -bin2fp(IR_BUFFER_RECV);
            stage += 1;

            if (IR_BUFFER_RECV == DEFEAT && win == 1){
                //player have win the game
                win = 0;
                ir_send_stop();
            }
        }

        else if (stage == 2){
            // reverse the velocity at y direction
            init_v.y = -bin2fp(IR_BUFFER_RECV);
            stage = 0;
            timeout = 0;
            ball_init(init_p, init_v);
        }
        IR_BUFFER_RECV = IDLE;
        led_set(LED1, 0);
    }
    

}

void ir_send_defeat(void){
    /**
     * Pause the sending of queue and push defeat message twice
     * to the sending queue. Continue afterwards
     */
    ir_send_queue_pause();
    ir_send(DEFEAT);
    ir_send(DEFEAT);
    ir_send_queue_continue();
}

