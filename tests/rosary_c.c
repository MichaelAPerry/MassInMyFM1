/* Build with: cc -std=c99 -Wall -Wextra -Werror tests/rosary_c.c -o /tmp/missa-rosary && /tmp/missa-rosary */
#include <assert.h>
#include <string.h>
#include "../app/rosary_steps.h"
int main(void) {
    assert(missa_rosary_length(1)==80);
    assert(missa_rosary_length(0)==75);
    assert(strcmp(missa_rosary_prayer(0,1),"SIGN OF CROSS")==0);
    assert(strcmp(missa_rosary_prayer(6,1),"GLORY BE")==0);
    assert(strcmp(missa_rosary_prayer(7,1),"ANNOUNCE MYSTERY")==0);
    assert(strcmp(missa_rosary_prayer(19,1),"GLORY BE")==0);
    assert(strcmp(missa_rosary_prayer(20,1),"FATIMA PRAYER")==0);
    assert(strcmp(missa_rosary_prayer(79,1),"SIGN OF CROSS")==0);
    assert(strcmp(missa_rosary_prayer(74,0),"SIGN OF CROSS")==0);
    assert(missa_rosary_decade(7,1)==1);
    assert(missa_rosary_decade(21,1)==2);
    assert(missa_rosary_decade(77,1)==5);
    assert(missa_rosary_bead(9,1)==1);
    assert(missa_rosary_bead(18,1)==10);
    for(int fatima=0;fatima<=1;fatima++)
        for(int i=0;i<missa_rosary_length((uint8_t)fatima);i++)
            assert(missa_rosary_prayer((uint8_t)i,(uint8_t)fatima)[0]!='\0');
    return 0;
}
