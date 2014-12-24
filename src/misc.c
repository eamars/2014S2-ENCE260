/** @file   misc.c
    @author Ran Bao (rba90) & Kane Williams (pkw21), Group1
    @date   8 October 2014
    @brief  Miscellaneous functions.
*/

#include "misc.h"
#include "system.h"
#include <math.h>

int8_t mod(int8_t a, int8_t b){
    int8_t ret = a % b;
    if (ret < 0){
        ret += b;
    }
    return ret;
}

/**
 * Encode and decode between float and unsigned integer
 * The method follows the IEEE standard
 * Inspired from http://www.exploringbinary.com/converting-floating-point-numbers-to-binary-strings-in-c/


 * The float number is represented as a 8 bit pattern, 
 * first 4 bit represents the integer part along with sign,
 * last 4 bits represents the fraction part.
 *
 * Example:
 * bin(1110101) represents 7.3215, while we only needs to transmit 
 * dec(117) through IR
 */

int ipow(int base, int exp)
{
    int result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }

    return result;
}

uint8_t fp2bin(float fp){
    // get the sign of fp
    uint8_t sign = (fp < 0);
    // get the value for integer part
    // the value of integer part * 16 represents the MSB
    uint8_t s;

    if (sign){
        fp = -fp;
        s = (uint8_t)fp * 16;
        s |= (1 << 7);
    }
    else{
        s = (uint8_t)fp * 16;
    }
    

    // break the float number into integer part and float part
    float intpart;
    float frac = modff(fp, &intpart); //C99 function
    
    uint8_t count = 0;
    while (frac > 0 && count < 4){
        frac *= 2;
        frac = modff(frac, &intpart);
        s += (uint8_t)intpart * ipow(2, (4 - count - 1));
        count ++;
    } 
    
    return s;
}

float bin2fp(uint8_t bp){
    uint8_t reminder;
    int8_t power = -4;
    float s = 0;

    while (power < 3){
        reminder = bp % 2;
        bp = bp / 2;
        s += reminder * powf(2, power);
        power += 1;
    }

    if (bp){
        return -s;
    }

    return s;
}



float distance(Vector *v1, Vector *v2){
    float dx = v1->x - v2->x;
    float dy = v1->y - v2->y;
    return sqrtf(dx * dx + dy * dy);
}

void normalize(Vector *v){
    float length = sqrtf(v->x * v->x + v->y * v->y);
    v->x /= length;
    v->y /= length;
}

float dot_product(Vector *v1, Vector *v2){
    return v1->x * v2->x + v1->y * v2->y;
}

int8_t rounds(float f){
    return (int8_t) roundf(f);
}
