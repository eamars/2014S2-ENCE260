/** @file   communication.h
    @author Ran Bao (rba90) & Kane Williams (pkw21), Group1
    @date   7 October 2014
    @brief  communication header file
*/

#ifndef COMM_H
#define COMM_H

#include "system.h"
#include "game.h" //ball struct

uint8_t ir_buffer_send;
uint8_t ir_buffer_recv;

#define IR_BUFFER_SEND ir_buffer_send
#define IR_BUFFER_RECV ir_buffer_recv
#define IR_RATE 150



/**
 * Packet headers
 */
#define IDLE 0xFF // 255
#define DEFEAT 0xFE // 254

/**
 * Player seed ranging from 0 to 255
 */
uint8_t player_seed;
void player_init(void);


/**
 * Background tasks that send and receive the buffer
 */
void ir_send_daemon(void);

/**
 * Use Queue to buffer the ir send job.
 * r_send_queue_daemon is able to send
 * continous message.
 */
void ir_send_queue_daemon(void);

/**
 * Send message to ir buffer and ready to send
 * Note: The message rate should greater or equal
 *       to IR_RATE. Otherwise the message would
 *       be break.
 * @param msg 8bit pattern that needs to be sent.
 */
void ir_send(uint8_t msg);

/**
 * Stop or continue the ir send task
 */
void ir_send_queue_pause(void);
void ir_send_queue_continue(void);

/**
 * Stop the ir_send_queue_daemon if the queue is clean
 */
void ir_send_stop(void);

/**
 * daemon task that receive player_seed from other,
 * exit the task if successfully assign the player id
 */
void player_recv_seed(void);

/**
 * Press the button to send player_seed
 */
void player_send_seed(void);


/**
 * Send the ball through IR
 * @param ball Target ball
 */
void ir_send_ball(Ball *ball);

/**
 * ball receive daemon that listen to ir
 * for any incoming balls
 */
void ir_recv_ball(void);

/**
 * Send defeat message to opponent 
 */
void ir_send_defeat(void);


#endif