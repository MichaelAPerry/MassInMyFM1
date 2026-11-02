# MISSA — Mass in a Box

Open-source Roman Catholic musical prayerbook and instrument for the M-VAVE FM-1.

**Status: early development / not installable.** This repository will use [fm1-base](https://github.com/charlesvestal/fm1-base) (MPL-2.0) as its firmware platform. No firmware image or physical hardware validation is claimed.

## First milestone

The project begins with an independently testable U.S. Catholic date engine, Rosary progression, and a small FM-1 application shell. The four modes are **Missa**, **Calendarium**, **Cantus**, and **Rosarium**. Canonical scores will not be labeled verified until musical source, transcription and rights have been checked.

## Architecture

- `app/` contains the app for the upstream `fm1-base` build system (see `docs/BUILD.md`).
- `missa/` contains host-testable, dependency-free Catholic calendar and prayer logic.
- `tests/` contains Python unit tests.

## Run tests

```bash
python3 -m unittest discover -s tests -v
```

## Safety and liturgical scope

For devotion and music study only; **never** a substitute for attendance at Mass. The app is not a sacramental simulation. Calendar algorithms are incomplete until validated against published USCCB calendars. Device clock is manually set; no onboard real-time clock is presumed.
