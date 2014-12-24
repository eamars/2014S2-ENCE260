/** @file   misc.h
    @author Ran Bao (rba90) & Kane Williams (pkw21), Group1
    @date   7 October 2014
    @brief  some support function header file
*/

#ifndef MISC_H
#define MISC_H

#include "system.h"
#include "game.h" // vector
/**
 * Find the real mod of a and b
 */
int8_t mod(int8_t a, int8_t b);


// encode and decode float number to unsigned integer
uint8_t fp2bin(float fp);
float bin2fp(uint8_t bp);

/**
 * Find the distance between each two coordinates
 * @param  v1 point1
 * @param  v2 point2
 * @return    distance in float
 */
float distance(Vector *v1, Vector *v2);

/**
 * Scale the vector to it's unit vector
 * @param v target vector
 */
void normalize(Vector *v);

/**
 * Calculate the dot product of two vectors
 * @param  v1 Vector1
 * @param  v2 Vector2
 * @return    dot product of two vectors in float
 */
float dot_product(Vector *v1, Vector *v2);

/**
 * Round float number
 * if f < x.5 then rounds down, else rounds up
 * @param  f float input
 * @return   integer output
 */
int8_t rounds(float f);

#endif
