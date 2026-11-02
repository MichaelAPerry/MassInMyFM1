# MISSA music milestone — sourced scores and polyphonic playback

## Current playable repertoire

### Salve Regina — simple tone

- Source: [GregoBase 18969](https://gregobase.selapa.net/chant.php?id=18969)
- Historical source cited there: *Antiphonale Monasticum*, 1934
- GregoBase transcription: Sancti Fructuosi, proofread February 2024; GregoBase GABC data licensed [CC0](https://gregobase.selapa.net/?page_id=2)
- Mode: V as indicated by the source
- 145 pitch events (290 MIDI-equivalent note-on/off transitions)
- Conversion: `tools/gabc_to_events.py` to `app/salve_regina_score.h`
- Status: source-identified authentic melody, playable in a *simplified equal-note rhythmic interpretation*. Phrase divisions have explicit pauses; advanced neumes and editorial rhythm signs are not reconstructed. We have not yet completed note-by-note comparison against the printed staff.

### Holy God, We Praise Thy Name — GROSSER GOTT

- Catholic origin: German tune from *Katholisches Gesangbuch*, 1774
- Historical setting: *Hymns Ancient and Modern*, 1869, as identified by [Church Posting / Open Hymnal's score](https://churchposting.com/hymns/holy-god-we-praise-thy-name/). The same melody and a separate 1819 public-domain four-part harmonization are also [cataloged at Timeless Truths](https://library.timelesstruths.org/music/Holy_God_We_Praise_Your_Name/midi/); do not confuse the two harmonizations.
- Four independently encoded parts in `data/hymns/holy-god-grosser-gott-1869.json`, 32 measures each, in F major and 3/4.
- Soprano 72 notes; alto 64; tenor 64; bass 70. Total 270 notes / 540 transitions.
- Duration: 96 quarter-note beats, default 100 BPM.
- Source score is historical and described as US public domain; the MISSA event transcription is provided under CC0. No modern copyrighted English liturgical texts or lyrics are bundled.
- Compiler: `tools/parts_to_events.py` to `app/holy_god_score.h`.
- **Not** a mandated proper chant for every Mass. It is an optional traditional hymn.

## Playback architecture

`app/music_engine.h`: bounded eight-voice synthesizer event scheduler. Score represents absolute time in ticks (96 quarter-note ticks), MIDI pitch, velocity, note-on/off, and independent part. Playback is sample-clock-based at 44,100 Hz. At equal ticks, note-offs are processed before note-ons; notes are silenced on stop and score end. Voice stealing is deterministic when all eight voices are occupied.

`app/app.c`: three synthetic timbral options (organ-like additive, vowel-like, bell-like additive). These are lightweight electronic approximations, not sampled organ pipes, natural choir voices, or sung syllables. The system supports live keyboard playing while a score is active (currently one simultaneous live key).

## FM-1 controls (Cantus)

- `SEL`: cycle the four modes
- `K1` encoder: choose Salve Regina / Holy God / engineering demo **while stopped**
- `PLAY`: start or pause/resume
- `REC`: stop and silence score voices
- `PRESET` encoder: choose all parts or individual S/A/T/B **while stopped**; for chant, all/melody
- `ALGO` encoder: change tempo before playback (40–200 BPM)
- `FX`: change synthesizer timbre during playback
- FM-1 keyboard: play a live electronic tone alongside music
- `SAVE`: save settings (the current score and tempo are not yet persistent)

## Reproduce and test

```sh
python3 tools/gabc_to_events.py data/chants/salve-regina-simple-18969.gabc app/salve_regina_score.h --check
python3 tools/parts_to_events.py data/hymns/holy-god-grosser-gott-1869.json app/holy_god_score.h --check
python3 -m unittest discover -s tests -v
cc -std=c99 -Wall -Wextra -Werror tests/music_c.c -o /tmp/missa-music && /tmp/missa-music
```

Firmware host compilation and scripted audio/navigation tests run through GitHub Actions: `.github/workflows/host.yml` using the external `charlesvestal/fm1-base` host target.

## Next steps / limits

1. Obtain device SDK/toolchain, build and inspect a `.fwsc` package; no device firmware package or physical flash test has been performed.
2. Verify the chant event transcription note-by-note against the historical staff, then implement richer neume phrasing and flexible rests.
3. Reconcile the exact SATB 1869 harmonization against a printed historical source; add regression checks against an independently imported MIDI/XML.
4. Implement user volume mixer/attack-release per voice and check real-device CPU/audio budgets.
5. Add licensed/verified Mass ordinary settings, day-to-score mapping, and the full U.S. liturgical calendar.
6. Bring a complete responsive Rosary prayer engine and saved mystery-set selection onto the device, not just the host reference and current prayer-label counter.
