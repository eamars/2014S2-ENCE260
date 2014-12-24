/** @file   framebuffer.c (MODIFIED)
    @author Ran Bao (rba90) & Kane Williams (pkw21), Group1
    @date   30 September 2014
    @brief  Displays on dot matrix.
*/

/**
 * this is a copy of display.c and lecmat.c but using matrix instead of using bit pattern
 */

#include "system.h"
#include "framebuffer.h"
#include "pio.h"


// all matrix stuff are in system.h
static const pio_t ledmat_rows[] =
{
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO, 
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};


/** Define PIO pins driving LED matrix columns.  */
static const pio_t ledmat_cols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};

/**
 * fb0: current framebuffer
 * fb1: stack framebuffer
 */
static uint8_t fb0[LEDMAT_COLS_NUM];
static uint8_t fb1[LEDMAT_COLS_NUM];

void fb_init (void)
{
    /* initilize all row and cols to high */
    uint8_t row;
    uint8_t col;

    for (row = 0; row < LEDMAT_ROWS_NUM; row++)
    {
        /* The rows are active low so configure PIO as an initially
           high output.  */
        pio_config_set (ledmat_rows[row], PIO_OUTPUT_HIGH);
    }

    for (col = 0; col < LEDMAT_COLS_NUM; col++)
    {
        /* The columns are active low so configure PIO as an initially
           high output.  */
        pio_config_set (ledmat_cols[col], PIO_OUTPUT_HIGH);
    }

    fb_clear();
}

void fb_clear(void){
    /* clear framebuffer0 */
    for (uint8_t col = 0; col < LEDMAT_COLS_NUM; col++){
        fb0[col] = 0x00;
    }
}

void fb_set_pixel(uint8_t col, uint8_t row, uint8_t state){
    if (col >= LEDMAT_COLS_NUM || row >= LEDMAT_ROWS_NUM){
        return;
    }
    uint8_t bitmask = BIT (row);
    uint8_t pattern = fb0[col] & ~bitmask;

    if (state){
        pattern |= bitmask;
    }
    fb0[col] = pattern;
}

void fb_set_col(uint8_t col, uint8_t pattern){
    if (col >= LEDMAT_COLS_NUM){
        return;
    }

    fb0[col] = pattern;
}

uint8_t fb_get_pixel(uint8_t col, uint8_t row){
    if (col >= LEDMAT_COLS_NUM || row >= LEDMAT_ROWS_NUM)
        return 0;

    uint8_t bitmask = BIT (row);

    return (fb0[col] & bitmask) != 0;
}




void __fb_flush_col(uint8_t col){
    static uint8_t prev_col = 0;

    uint8_t pattern = fb0[col];

    /* disable previous col */
    pio_output_high (ledmat_cols[prev_col]);

    /* active desired row */
    for (uint8_t row = 0; row < LEDMAT_ROWS_NUM; row++)
    {
        /* The rows are active low.  
            we have 7 rows
        */

        if (pattern & 1)
            pio_output_low (ledmat_rows[row]);
        else
            pio_output_high (ledmat_rows[row]);

        pattern >>= 1;
    }

    /* active desired col */
    pio_output_low (ledmat_cols[col]);

    /* record prev col */
    prev_col = col;
}

void fb_flush(void){
    static uint8_t col = 0;

    __fb_flush_col(col);

    col += 1;

    if (col >= LEDMAT_COLS_NUM){
        col = 0;
    }
}

void fb_push(void){
    for (uint8_t col = 0; col < LEDMAT_COLS_NUM; col++){
        fb1[col] = fb0[col];
    }
}

void fb_pop(void){
    for (uint8_t col = 0; col < LEDMAT_COLS_NUM; col++){
        fb0[col] = fb1[col];
    }
}

void fb_fill(void){
    for (uint8_t col = 0; col < LEDMAT_COLS_NUM; col++){
        fb0[col] = 0x7F;
    }
}

