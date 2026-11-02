import json
import unittest
from pathlib import Path
from tools.gabc_to_events import convert as convert_gabc
from tools.parts_to_events import to_events

ROOT=Path(__file__).resolve().parents[1]

class ScoreTests(unittest.TestCase):
    def test_salve_source(self):
        data=(ROOT/"data/chants/salve-regina-simple-18969.gabc").read_text(encoding="utf-8")
        events,end=convert_gabc(data)
        self.assertEqual(len(events),290)
        self.assertEqual(events[0][:2],(0,65))
        self.assertEqual(events[-1][1],65)
        self.assertEqual(end,5776)
        self.assertTrue(all(events[i][0]<=events[i+1][0] for i in range(len(events)-1)))
    def test_salb_chant_only_single_part(self):
        ev,_=convert_gabc((ROOT/"data/chants/salve-regina-simple-18969.gabc").read_text(encoding="utf-8"))
        self.assertEqual({e[4] for e in ev},{0})
    def test_satb_metadata_and_duration(self):
        data=json.loads((ROOT/"data/hymns/holy-god-grosser-gott-1869.json").read_text(encoding="utf-8"))
        ev,end=to_events(data)
        self.assertEqual(end,9216)
        self.assertEqual(len(ev),540)
        self.assertEqual({x[4] for x in ev},{0,1,2,3})
        first=[x for x in ev if x[0]==0 and x[3]]
        self.assertEqual({(x[4],x[1]) for x in first},{(0,65),(1,60),(2,57),(3,53)})
        self.assertTrue(all(0<=x[1]<=127 and 0<=x[0]<=end for x in ev))
    def test_harmony_parts_independent(self):
        data=json.loads((ROOT/"data/hymns/holy-god-grosser-gott-1869.json").read_text(encoding="utf-8"))
        ev,_=to_events(data)
        self.assertEqual([sum(x[3] for x in ev if x[4]==p) for p in range(4)],[72,64,64,70])
    def test_bad_meter_fails(self):
        data=json.loads((ROOT/"data/hymns/holy-god-grosser-gott-1869.json").read_text(encoding="utf-8"))
        data["voices"]["S"]["bars"][0]="f:1"
        with self.assertRaises(ValueError):
            to_events(data)

if __name__=="__main__":unittest.main()
