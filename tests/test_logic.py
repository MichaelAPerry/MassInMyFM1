import unittest
from datetime import date
from missa.calendar import easter, advent_start, observance, cycles
from missa.rosary import MYSTERIES, Rosary, sequence

class CalendarTests(unittest.TestCase):
    def test_easter(self):
        self.assertEqual(easter(2026), date(2026, 4, 5))
        self.assertEqual(easter(2025), date(2025, 4, 20))
        self.assertEqual(easter(2038), date(2038, 4, 25))
    def test_advent(self):
        self.assertEqual(advent_start(2026), date(2026, 11, 29))
        self.assertEqual(advent_start(2025), date(2025, 11, 30))
    def test_all_saints(self):
        self.assertEqual(observance(date(2026,11,1)).name, "All Saints")
    def test_christmas(self):
        self.assertEqual(observance(date(2026,12,25)).color,"white")
    def test_cycle(self):
        self.assertEqual(cycles(date(2026,12,1))[0], "B")

class RosaryTests(unittest.TestCase):
    def test_mysteries(self):
        self.assertEqual(len(MYSTERIES),4)
        self.assertTrue(all(len(x)==5 for x in MYSTERIES.values()))
    def test_five_decades(self):
        s=sequence()
        self.assertEqual(s.count("Hail Mary"), 53)
        self.assertEqual(s.count("Our Father"), 6)
        self.assertEqual(s.count("Fatima Prayer"), 5)
    def test_navigation(self):
        r=Rosary()
        r.previous()
        self.assertEqual(r.position,0)
        r.next()
        self.assertEqual(r.current,"Apostles' Creed")
        for _ in range(1000): r.next()
        self.assertTrue(r.completed)
        r.previous()
        self.assertFalse(r.completed)

if __name__=="__main__": unittest.main()
