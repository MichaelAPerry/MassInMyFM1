/* SPDX-License-Identifier: MIT
 * MISSA Rosary: canonical indexed sequence, optional Fatima invocation.
 * 79 steps with Fatima, 74 without. Each invocation is a user-paced step.
 */
#pragma once
#include <stdint.h>
enum { MISSA_ROSARY_INTRO = 6, MISSA_ROSARY_DECADE_FATIMA = 14, MISSA_ROSARY_DECADE_PLAIN = 13 };
static uint8_t missa_rosary_length(uint8_t fatima) {return (uint8_t)(6u+5u*(fatima?14u:13u)+3u);}
static uint8_t missa_rosary_decade(uint8_t position,uint8_t fatima) {
    if(position<6u)return 0u;
    uint8_t step=(uint8_t)(position-6u), width=fatima?14u:13u;
    return step>=5u*width?5u:(uint8_t)(1u+step/width);
}
static uint8_t missa_rosary_bead(uint8_t position,uint8_t fatima) {
    if(position<6u)return position==2u?1u:(position>=3u && position<=5u?position-2u:0u);
    uint8_t width=fatima?14u:13u, step=(uint8_t)(position-6u);
    if(step>=5u*width)return 0u;
    uint8_t local=step%width;
    return local>=2u && local<12u ? (uint8_t)(local-1u) : 0u;
}
static const char *missa_rosary_prayer(uint8_t position,uint8_t fatima) {
    static const char *const intro[6]={"SIGN OF CROSS","APOSTLES CREED","OUR FATHER","HAIL MARY 1/3","HAIL MARY 2/3","HAIL MARY 3/3"};
    if(position<6u)return intro[position];
    uint8_t width=fatima?14u:13u, step=(uint8_t)(position-6u);
    if(step>=5u*width) {
        static const char *const ending[3]={"HAIL HOLY QUEEN","CONCLUDING PRAYER","SIGN OF CROSS"};
        uint8_t idx=(uint8_t)(step-5u*width);
        return idx<3u?ending[idx]:"FINISHED";
    }
    uint8_t local=step%width;
    if(local==0u)return "ANNOUNCE MYSTERY";
    if(local==1u)return "OUR FATHER";
    if(local<12u)return "HAIL MARY";
    if(local==12u)return "GLORY BE";
    return "FATIMA PRAYER";
}
