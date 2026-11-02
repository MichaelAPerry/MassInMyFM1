# MISSA — Mass in a Box

A Roman Catholic musical prayerbook, devotional Rosary companion, and tiny sacred-music instrument for the M-VAVE FM-1.

**Status:** FM-1 host simulator builds and scripted audio/navigation tests pass. **Not yet device-tested or released as an installable firmware image.** Developed on [fm1-base](https://github.com/charlesvestal/fm1-base) (MPL-2.0 upstream).

## Current modes

- **Missa** — shell for the Mass-oriented interface. Its complete Order-of-Mass music list and links to the USCCB calendar remain unfinished.
- **Calendarium** — manually selectable dates; Python reference Gregorian Easter/Advent and partial/provisional observances, **not yet** a completed device observance engine.
- **Cantus** — plays sourced pitch transcriptions of *Salve Regina* (simple tone, Gregorian chant) and *Holy God, We Praise Thy Name* (historic 4-part GROSSER GOTT harmony). Offers organ-like, choir-like and bell-like synthetic timbres, tempo, start/pause/stop, and SATB part isolation.
- **Rosarium** — device prayer labels and previous/next progress; Python reference defines all 20 mysteries, and C includes the complete 80-step sequence (including optional Fatima prayers). Device screen does not yet present full metadata, seasonal rules, or auto-saving every step.

## Sources and license

Both musical works are identifiable historical compositions, not generative imitations. The Salve Regina transcription is CC0 from [GregoBase](https://gregobase.selapa.net/chant.php?id=18969), but its timing is currently an **editorial equal-note adaptation** rather than an authoritative chant-rhythm reconstruction. GROSSER GOTT uses a historic public-domain four-part setting derived from printed/historical music data and identified in [the Open Hymnal/Church Posting source](https://churchposting.com/hymns/holy-god-we-praise-thy-name/). See [docs/MUSIC.md](docs/MUSIC.md) for full provenance and limitations.

## Repository

- `app/` — native FM-1 C app, audio engine, symbolic scores, tiny UI, Rosary step logic.
- `missa/` — portable Catholic reference calendar and Rosary models.
- `data/chants/`, `data/hymns/` — traceable symbolic source notation and metadata.
- `tools/` — deterministic score compilers; do not parse XML/GABC on the device.
- `tests/` — C/Python unit suites and scripted FM-1 host emulator interaction tests.
- `docs/MUSIC.md` — music features, provenance, timing choices and controls.
- `docs/BUILD.md` — host framework and installation caveats.

## Run reference tests

```sh
python3 -m unittest discover -s tests -v
cc -std=c99 -Wall -Wextra -Werror tests/music_c.c -o /tmp/missa-music && /tmp/missa-music
cc -std=c99 -Wall -Wextra -Werror tests/rosary_c.c -o /tmp/missa-rosary && /tmp/missa-rosary
```

CI workflows: [reference tests](.github/workflows/tests.yml), [FM-1 host simulator and scripted audio test](.github/workflows/host.yml).

## Scope and safety

Designed for prayer, listening and liturgical study, never to simulate the Eucharist or substitute for attendance at Mass. Current U.S. Catholic calendar rules are incomplete and provisional. There is no assumed battery-backed RTC; users manually select the date. Physical FM-1 validation, toolchain build, formal licensing audit and installable release are outstanding.
