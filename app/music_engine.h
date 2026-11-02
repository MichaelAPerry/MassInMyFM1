/* SPDX-License-Identifier: MIT
 * Bounded polyphonic event sequencer. Score ticks are 1/96 quarter notes.
 * All note events are processed on the audio thread, no allocation or I/O.
 */
#pragma once
#include <stdint.h>
#include <string.h>
#define MISSA_VOICES 8
typedef struct { uint16_t tick; uint8_t note, velocity, on, part; } missa_event;
typedef struct {uint8_t note, part, velocity, active; float phase, env;} missa_voice;
typedef struct {
    missa_voice voices[MISSA_VOICES];
    const missa_event *events;
    uint16_t count, next, tempo, score_end;
    uint32_t sample_clock, sample_at_event;
    uint8_t playing, paused, timbre, part_mode;
} missa_player;
static void missa_player_clear(missa_player *p) {
    memset(p->voices,0,sizeof p->voices);
    p->playing=0;p->paused=0;p->next=0;p->sample_clock=0;p->sample_at_event=0;
}
static void missa_player_start(missa_player *p,const missa_event *ev,uint16_t count,uint16_t bpm,uint16_t end) {
    missa_player_clear(p);
    p->events=ev;p->count=count;p->tempo=bpm?bpm:96;p->score_end=end;
    p->playing=(uint8_t)(ev!=0 && count!=0 && end!=0);
}
static void missa_player_note(missa_player *p,uint8_t note,uint8_t vel,uint8_t part,uint8_t on) {
    uint32_t i;
    if(!on || !vel) {
        for(i=0;i<MISSA_VOICES;i++)
            if(p->voices[i].active && p->voices[i].note==note && p->voices[i].part==part) {
                p->voices[i].active=0;return;
            }
        return;
    }
    for(i=0;i<MISSA_VOICES;i++)if(!p->voices[i].active)break;
    if(i==MISSA_VOICES)i=0; /* deterministic steal, release old note */
    p->voices[i]=(missa_voice){note,part,vel,1,0.0f,0.0f};
}
static uint32_t missa_tick_sample(uint32_t tick,uint16_t bpm) {
    return (uint32_t)(((uint64_t)tick*44100u*60u)/(96u*(bpm?bpm:96u)));
}
static void missa_player_step(missa_player *p) {
    if(!p->playing || p->paused)return;
    while(p->next<p->count && missa_tick_sample(p->events[p->next].tick,p->tempo)<=p->sample_clock) {
        const missa_event *e=&p->events[p->next++];
        if(p->part_mode==0u || e->part+1u==p->part_mode)
            missa_player_note(p,e->note,e->velocity,e->part,e->on);
    }
    if(p->next==p->count && p->sample_clock>=missa_tick_sample(p->score_end,p->tempo)) {
        missa_player_clear(p);return;
    }
    p->sample_clock++;
}
