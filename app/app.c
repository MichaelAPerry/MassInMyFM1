/* SPDX-License-Identifier: MIT
 * MISSA M0 device shell built for fm1-base platform; no liturgical music yet.
 * This source uses the platform's unity-build graphical API.
 */
#include <stdint.h>
#include <string.h>
#include "fastmath.h"
#define MISSA_MAGIC 0x4153534Du
typedef struct {
    uint32_t magic;
    uint16_t year;
    uint8_t month, day, mode, rosary_pos, tone;
} missa_settings_t;
static missa_settings_t settings;
static uint32_t previous_buttons;
static uint8_t redraw;
static float phase, level;
static int32_t live_note = -1;
static uint32_t held_keys;
static const char *const mode_name[4] = {"MISSA / MASS", "CALENDARIUM", "CANTUS", "ROSARIUM"};
static const char *const mystery_name[4] = {"JOYFUL", "LUMINOUS", "SORROWFUL", "GLORIOUS"};
static uint8_t mystery_set;
static uint8_t is_leap(uint32_t y) { return (uint8_t)((y%4u == 0u && y%100u != 0u) || y%400u == 0u); }
static uint8_t dim(uint32_t y,uint32_t m) {
    static const uint8_t days[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    return (uint8_t)(days[m-1u] + (m==2u && is_leap(y)));
}
static void date_step(int dir) {
    if (dir > 0) {
        if (++settings.day > dim(settings.year,settings.month)) {
            settings.day=1;
            if (++settings.month>12) {settings.month=1;settings.year++;}
        }
    } else {
        if (settings.day>1) settings.day--;
        else {
            if (settings.month==1) {settings.month=12;if (settings.year>1900) settings.year--;}
            else settings.month--;
            settings.day=dim(settings.year,settings.month);
        }
    }
    redraw=1;
}
static void save(void) { (void)plat_store_save(OBJ_SETTINGS,&settings,sizeof settings); }
void app_init(void) {
    if (plat_store_load(OBJ_SETTINGS,&settings,sizeof settings)!=(int)sizeof settings ||
        settings.magic!=MISSA_MAGIC || settings.year<1900 || settings.year>2199 ||
        settings.month<1 || settings.month>12 || settings.day<1 ||
        settings.day>dim(settings.year,settings.month) || settings.mode>3 || settings.tone>2) {
        settings=(missa_settings_t){MISSA_MAGIC,2026,11,2,0,0,0};
    }
}
void app_panic(void) {live_note=-1; level=0.0f;}
void app_render(int32_t *lr,uint32_t n,uint32_t master_q12) {
    float inc=live_note>=0 ? 440.0f*fm_exp2f((float)(live_note-69)/12.0f)/44100.0f : 0.0f;
    float gain=(float)master_q12/4096.0f*2600000.0f;
    for(uint32_t i=0;i<n;i++) {
        float target=live_note>=0 ? 1.0f : 0.0f;
        level+=(target-level)*0.002f;
        phase+=inc;
        if(phase>=1.0f)phase-=1.0f;
        float s = settings.tone==0 ? fm_sinf(phase*6.2831853f) :
                  settings.tone==1 ? (phase<0.5f?1.0f:-1.0f)*0.45f :
                  4.0f*fm_fabsf(phase-0.5f)-1.0f;
        int32_t sample=(int32_t)(s*level*gain);
        lr[2*i]=sample;lr[2*i+1]=sample;
    }
}
void app_say(const char *a,const char *b) {(void)a;(void)b;redraw=1;}
static void input(void) {
    uint32_t buttons=plat_buttons(), keys=plat_keys();
    uint32_t pressed=buttons & ~previous_buttons;
    int32_t d=plat_enc(EN_SELECT);
    (void)plat_enc(EN_ALGO);
    (void)plat_enc(EN_K1);(void)plat_enc(EN_K2);(void)plat_enc(EN_K3);(void)plat_enc(EN_K4);
    if (settings.mode==1 && d) { int step=d>0?1:-1;while(d){date_step(step);d-=step;} }
    else if (settings.mode==3 && d) {
        int n=(int)settings.rosary_pos+d;
        settings.rosary_pos=(uint8_t)clamp(n,0,79);
        redraw=1;
    } else if(d) {settings.mode=(uint8_t)clamp((int)settings.mode+d,0,3);redraw=1;}
    if (pressed & (1u<<B_HOME)) {settings.mode=0;redraw=1;}
    if (pressed & (1u<<B_SEL)) {settings.mode=(settings.mode+1u)%4u;redraw=1;}
    if (pressed & (1u<<B_EDIT)) {mystery_set=(mystery_set+1u)%4u;redraw=1;}
    if (pressed & (1u<<B_PLAY) && settings.mode==3 && settings.rosary_pos<79) {settings.rosary_pos++;redraw=1;}
    if (pressed & (1u<<B_OCTDN)) date_step(-1);
    if (pressed & (1u<<B_OCTUP)) date_step(1);
    if (pressed & (1u<<B_SAVE)) save();
    if (keys!=held_keys) {
        if(keys) {for(int k=0;k<NKEYS;k++) if(keys & (1u<<k)) {live_note=53+k;break;}}
        else live_note=-1;
        held_keys=keys;redraw=1;
    }
    previous_buttons=buttons;
    plat_leds(buttons,keys);
}
void app_input_only(void) {input();}
void app_ui_init(void) {lcd_fill(0,0,240,240,C_BLACK);redraw=1;}
void app_frame(void) {
    char date_text[32],number[12];
    input();
    if(!redraw)return;
    redraw=0;
    cv_begin(240,240,C_BLACK);
    cv_rect(0,0,240,36,RGB(105,22,48));
    cv_text(12,8,&FONT_B,"MISSA",C_WHITE);
    cv_text(12,44,&FONT_B,mode_name[settings.mode],C_WHITE);
    date_text[0]=(char)('0'+settings.month/10);
    date_text[1]=(char)('0'+settings.month%10);date_text[2]='/';
    date_text[3]=(char)('0'+settings.day/10);date_text[4]=(char)('0'+settings.day%10);date_text[5]='/';
    for(int i=0;i<4;i++)date_text[6+i]=(char)('0'+(settings.year/(i==0?1000:i==1?100:i==2?10:1))%10);
    date_text[10]=0;
    cv_text(12,76,&FONT_B,date_text,C_WHITE);
    if(settings.mode==3) {
        cv_text(12,110,&FONT_XS,mystery_name[mystery_set],C_WHITE);
        number[0]='P';number[1]='R';number[2]='A';number[3]='Y';number[4]=' ';
        number[5]=(char)('0'+settings.rosary_pos/10);
        number[6]=(char)('0'+settings.rosary_pos%10);number[7]=0;
        cv_text(12,134,&FONT_B,number,C_WHITE);
        cv_text(12,169,&FONT_XS,"PLAY: NEXT / EDIT: SET",C_WHITE);
    } else if(settings.mode==2) {
        cv_text(12,110,&FONT_XS,"MUSIC CATALOG PENDING",C_WHITE);
        cv_text(12,132,&FONT_XS,"KEYS: LIVE SYNTH",C_WHITE);
    } else {
        cv_text(12,110,&FONT_XS,"CALENDAR: PROVISIONAL",C_WHITE);
        cv_text(12,132,&FONT_XS,"OCT +/-: DATE",C_WHITE);
    }
    cv_text(12,208,&FONT_XS,"SEL: MODE  SAVE: DATE",C_WHITE);
    cv_blit(0,0);
}
