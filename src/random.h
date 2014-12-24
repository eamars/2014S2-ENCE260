/** @file   random.h
    @author Ran Bao (rba90) & Kane Williams (pkw21), Group1
    @date   7 October 2014
    @brief  randon number generator header file
*/

#ifndef RANDOM_H
#define RANDOM_H

#include "system.h"


/**
 * Create a random number for srand() use
 * The random number is given by the random voltage when initilizing
 * the PD5 or PIN3 in GPIO port
 */
void random_init(void);

/**
 * Return the unsigned random number range from s to e, same as rand()
 * @param  s start point 
 * @param  e end point
 * @return   a random integer
 */
uint8_t randi(uint8_t s, uint8_t e);


#endif