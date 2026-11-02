"""Partial, explicitly scoped Roman Rite (U.S.) calendar reference engine.

This module intentionally returns provisional results for weekdays and does not
claim to implement the complete General Roman Calendar or local propers.
"""
from dataclasses import dataclass
from datetime import date, timedelta

@dataclass(frozen=True)
class Observance:
    date: date
    name: str
    rank: str
    season: str
    color: str
    sunday_cycle: str
    weekday_cycle: str
    verified: bool = False

def easter(year: int) -> date:
    """Gregorian computus."""
    a = year % 19
    b, c = divmod(year, 100)
    d, e = divmod(b, 4)
    f = (b + 8) // 25
    g = (b - f + 1) // 3
    h = (19 * a + b - d - g + 15) % 30
    i, k = divmod(c, 4)
    l = (32 + 2 * e + 2 * i - h - k) % 7
    m = (a + 11 * h + 22 * l) // 451
    month, day = divmod(h + l - 7 * m + 114, 31)
    return date(year, month, day + 1)

def advent_start(year: int) -> date:
    christmas = date(year, 12, 25)
    # Sunday nearest November 30, between Nov 27 and Dec 3.
    nov27 = date(year, 11, 27)
    return nov27 + timedelta(days=(6 - nov27.weekday()) % 7)

def cycles(d: date) -> tuple[str, str]:
    year = d.year + (d >= advent_start(d.year))
    return "ABC"[(year - 1) % 3], "II" if d.year % 2 == 0 else "I"

def observance(d: date) -> Observance:
    e = easter(d.year)
    ash = e - timedelta(days=46)
    palm = e - timedelta(days=7)
    pentecost = e + timedelta(days=49)
    advent = advent_start(d.year)
    prior_advent = advent_start(d.year - 1)
    christmas_end = date(d.year, 1, 6)  # only broad season approximation
    if d >= advent:
        season, color = "Advent", "violet"
    elif d >= date(d.year, 12, 25) or d <= christmas_end:
        season, color = "Christmas", "white"
    elif ash <= d < e:
        season, color = "Lent", "violet"
    elif e <= d <= pentecost:
        season, color = "Easter", "white"
    else:
        season, color = "Ordinary Time", "green"
    special = {
        e: ("Easter Sunday", "solemnity", "white", "Easter"),
        ash: ("Ash Wednesday", "weekday", "violet", "Lent"),
        palm: ("Palm Sunday of the Passion of the Lord", "sunday", "red", "Lent"),
        e - timedelta(days=3): ("Holy Thursday", "triduum", "white", "Triduum"),
        e - timedelta(days=2): ("Good Friday", "triduum", "red", "Triduum"),
        e - timedelta(days=1): ("Holy Saturday", "triduum", "violet", "Triduum"),
        pentecost: ("Pentecost Sunday", "solemnity", "red", "Easter"),
        date(d.year, 11, 1): ("All Saints", "solemnity", "white", season),
        date(d.year, 11, 2): ("Commemoration of All the Faithful Departed", "commemoration", "violet", season),
        date(d.year, 12, 25): ("Nativity of the Lord", "solemnity", "white", "Christmas"),
    }
    sunday, weekday = cycles(d)
    if d in special:
        name, rank, color, season = special[d]
        return Observance(d, name, rank, season, color, sunday, weekday)
    if d.weekday() == 6:
        name, rank = "Sunday (provisional; proper unimplemented)", "sunday"
    else:
        name, rank = "Weekday (provisional; memorials unimplemented)", "weekday"
    return Observance(d, name, rank, season, color, sunday, weekday)
