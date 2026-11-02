"""Catholic Rosary mystery catalog and deterministic, manual prayer advancement."""
from dataclasses import dataclass

MYSTERIES = {
 "Joyful": ("Annunciation", "Visitation", "Nativity", "Presentation", "Finding in the Temple"),
 "Luminous": ("Baptism in the Jordan", "Wedding at Cana", "Proclamation of the Kingdom", "Transfiguration", "Institution of the Eucharist"),
 "Sorrowful": ("Agony in the Garden", "Scourging at the Pillar", "Crowning with Thorns", "Carrying of the Cross", "Crucifixion"),
 "Glorious": ("Resurrection", "Ascension", "Descent of the Holy Spirit", "Assumption", "Coronation of Mary")
}
# Monday=0; configurable traditional seasonal Sunday variation not implemented.
WEEKDAY_SET = ("Joyful","Sorrowful","Glorious","Luminous","Sorrowful","Joyful","Glorious")

def sequence(fatima=True):
    steps = ["Sign of the Cross", "Apostles' Creed", "Our Father"]
    steps += ["Hail Mary"] * 3 + ["Glory Be"]
    for decade in range(5):
        steps += [f"Mystery {decade+1}", "Our Father"] + ["Hail Mary"] * 10 + ["Glory Be"]
        if fatima:
            steps += ["Fatima Prayer"]
    steps += ["Hail Holy Queen", "Concluding Prayer", "Sign of the Cross"]
    return tuple(steps)

@dataclass
class Rosary:
    mystery_set: str = "Joyful"
    position: int = 0
    fatima: bool = True
    def __post_init__(self):
        if self.mystery_set not in MYSTERIES:
            raise ValueError("unknown mystery set")
        if not 0 <= self.position < len(sequence(self.fatima)):
            raise ValueError("invalid position")
    @property
    def current(self): return sequence(self.fatima)[self.position]
    @property
    def completed(self): return self.position == len(sequence(self.fatima)) - 1
    def next(self): self.position = min(self.position + 1, len(sequence(self.fatima))-1)
    def previous(self): self.position = max(0, self.position-1)
