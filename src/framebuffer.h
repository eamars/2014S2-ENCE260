/** @file   framebuffer.h
    @author Ran Bao (rba90) & Kane Williams (pkw21), Group1
    @date   7 October 2014
    @brief  framebuffer header file
*/

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "system.h"
#include "pio.h"

/**
 * Redefine the ROW_NUM and COL_NUM for easy use
 */
#define ROW_NUM LEDMAT_ROWS_NUM
#define COL_NUM LEDMAT_COLS_NUM

/**
 * Sample screen flash rate at 300Hz, which refreshes each column at
 * 60Hz.
 */
#define DISPLAY_TASK_RATE 300


/**
 * Initilize the display task
 */
void fb_init (void);

/**
 * Clear the whole screen buffer
 */
void fb_clear(void);

/**
 * Set the state of pixel of framebuffer
 * @param col   col number, 0 < col < COL_NUM
 * @param row   row number, 0 < row < ROW_NUM
 * @param state state of pixel, 1 for on and 0 for off
 */
void fb_set_pixel(uint8_t col, uint8_t row, uint8_t state);

/**
 * Set the whole column by pattern
 * @param col     col number
 * @param pattern bit pattern such that 0 < pattern < 127
 */
void fb_set_col(uint8_t col, uint8_t pattern);

/**
 * The the state of pixel of framebuffer
 * @param col   col number, 0 < col < COL_NUM
 * @param row   row number, 0 < row < ROW_NUM
 * @return     state of pixel, 1 for on and 0 for off
 */
uint8_t fb_get_pixel(uint8_t col, uint8_t row);


/**
 * Execute the display task at DISPLAY_TASK_RATE
 */
void fb_flush(void);

/**
 * Backup the current framebuffer to second buffer
 */
void fb_push(void);

/**
 * Restore the current framebuffer to first buffer 
 */
void fb_pop(void);

/**
 * Opposite to fb_clear(), which fills the screen
 */
void fb_fill(void);


#endif