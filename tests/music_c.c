#include <assert.h>
#include "../app/music_engine.h"
int main(void) {
    missa_player p={0};
    const missa_event score[]={
        {0,60,100,1,0},{0,64,100,1,1},{0,67,100,1,2},
        {96,60,0,0,0},{96,64,0,0,1},{96,67,0,0,2}
    };
    missa_player_start(&p,score,6,120,192);
    missa_player_step(&p);
    assert(p.playing && p.next==3);
    assert(p.voices[0].active && p.voices[1].active && p.voices[2].active);
    assert(missa_tick_sample(96,120)==22050);
    while(p.sample_clock<=22050)missa_player_step(&p);
    assert(p.next==6);
    for(int i=0;i<3;i++)assert(!p.voices[i].active);
    p.paused=1;
    uint32_t frozen=p.sample_clock;
    for(int i=0;i<100;i++)missa_player_step(&p);
    assert(p.sample_clock==frozen);
    p.paused=0;
    while(p.playing)missa_player_step(&p);
    for(int i=0;i<MISSA_VOICES;i++)assert(!p.voices[i].active);
    missa_player_start(&p,score,6,120,192);
    for(int i=0;i<10;i++)missa_player_note(&p,(uint8_t)(40+i),100,0,1);
    assert(p.voices[0].note==49);
    for(int i=0;i<MISSA_VOICES;i++)assert(p.voices[i].active);
    missa_player_clear(&p);
    assert(!p.playing);
    return 0;
}
