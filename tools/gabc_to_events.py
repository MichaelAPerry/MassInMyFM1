#!/usr/bin/env python3
"""Build a deterministic FM-1 note-event array from limited GABC c4 monophony.

Conservative pitch-only interpretation: each note gets an equal short duration.
Complex signs are retained in the source but not interpreted rhythmically.
This is deliberately NOT a general-purpose Gregorian notation renderer.
"""
from __future__ import annotations
import argparse
from pathlib import Path
import re

# c4: c at F4, and steps of the F diatonic mode through j = F5.
MIDI = dict(zip("cdefghij", (65, 67, 69, 70, 72, 74, 76, 77)))
NOTE_TICKS = 32
GATE_TICKS = 28


def convert(text: str) -> tuple[list[tuple[int,int,int,int,int]], int]:
    if "%%" not in text:
        raise ValueError("missing GABC header terminator")
    body = text.split("%%",1)[1]
    groups = re.findall(r"\(([^)]*)\)", body)
    if not groups or groups[0] != "c4":
        raise ValueError("currently only initial c4 clef is supported")
    events: list[tuple[int,int,int,int,int]] = []
    tick = 0
    for group in groups[1:]:
        if not group:
            continue
        if group.startswith(("c1","c2","c3","c4","f1","f2","f3","f4")):
            raise ValueError("mid-score clef changes require manual handling")
        if group.startswith("::"):
            tick += 96
            continue
        if group.startswith(";"):
            tick += 64
            continue
        if group.startswith(":"):
            tick += 32
            continue
        if group.startswith("`"):
            tick += 16
            continue
        clean = re.sub(r"\[[^]]*\]", "", group)
        # Check for accidentals, key changes, and special glyphs not modeled.
        if any(x in clean for x in ("x","y","#")):
            raise ValueError(f"unsupported accidental/glyph in {group!r}")
        pitches = [c for c in clean if c in MIDI]
        # Markup that looks like a note but is part of an editorial instruction
        # must be parsed manually rather than passed through this importer.
        for ch in pitches:
            events.append((tick,MIDI[ch],100,1,0))
            events.append((tick+GATE_TICKS,MIDI[ch],0,0,0))
            tick += NOTE_TICKS
    if not events or tick > 65000:
        raise ValueError("empty or oversized chant")
    return sorted(events, key=lambda e:(e[0],e[3],e[1])), tick+32


def header(events: list[tuple[int,int,int,int,int]], end: int) -> str:
    lines = [
        "/* SPDX-License-Identifier: CC0-1.0 */",
        "/* Generated from GregoBase ID 18969, Salve Regina simple tone.",
        " * Equal-note timing is a MISSA editorial rendering; see source metadata.",
        " * DO NOT edit by hand: tools/gabc_to_events.py */",
        "#pragma once",
        '#include "music_engine.h"',
        "static const missa_event missa_salve_regina[] = {"
    ]
    for tick,note,vel,on,part in events:
        lines.append(f"    {{{tick},{note},{vel},{on},{part}}},")
    lines.extend((
        "};",
        f"#define MISSA_SALVE_END {end}u",
        "#define MISSA_SALVE_COUNT ((uint16_t)(sizeof missa_salve_regina / sizeof missa_salve_regina[0]))",
        "",
    ))
    return "\n".join(lines)


def main() -> None:
    ap=argparse.ArgumentParser()
    ap.add_argument("source",type=Path)
    ap.add_argument("output",type=Path)
    ap.add_argument("--check",action="store_true",help="assert output is up to date without writing")
    args=ap.parse_args()
    events,end=convert(args.source.read_text(encoding="utf-8"))
    result=header(events,end)
    if args.check:
        if args.output.read_text(encoding="utf-8") != result:
            raise SystemExit("generated chant header is stale")
    else:
        args.output.write_text(result,encoding="utf-8")
    print(f"Salve Regina: {len(events)//2} notes, {end} ticks")


if __name__=="__main__":
    main()
