# This is a simple program that converts odds to probabilities and calculates the expected value of a bet.

from __future__ import annotations
from dataclasses import dataclass
from typing import List, Dict, Tuple

# --- Conversions ---

def american_to_decimal(american: int) -> float:
    """Convert American odds to decimal (US odds to European decimal).
    +150 -> 2.50, -120 -> 1.8333...
    """
    if american > 0:
        return 1 + american / 100
    else:
        return 1 + 100 / abs(american)


def decimal_to_american(decimal_odds: float) -> int:
    if decimal_odds <= 1:
        raise ValueError("Decimal odds must be > 1")
    if decimal_odds >= 2:
        return int(round((decimal_odds - 1) * 100))
    else:
        return int(round(-100 / (decimal_odds - 1)))


def implied_prob_from_american(american: int) -> float:
    d = american_to_decimal(american)
    return 1 / d


def combine_decimal_odds(legs: List[float]) -> float:
    """Parlay decimal odds assuming independence (product of legs)."""
    out = 1.0
    for d in legs:
        out *= d
    return out

# --- No‑vig probability estimation ---

def normalize_probs(probs: List[float]) -> List[float]:
    """Proportional normalization to remove vig across outcomes of a market."""
    total = sum(probs)
    if total <= 0:
        raise ValueError("Sum of probabilities must be positive")
    return [p / total for p in probs]


def no_vig_from_american(americans: List[int]) -> List[float]:
    """Proportional no‑vig: convert to implied probs and renormalize."""
    raw = [implied_prob_from_american(a) for a in americans]
    return normalize_probs(raw)

# --- EV & Kelly ---

def expected_value(true_p: float, offered_decimal: float, stake: float = 1.0) -> float:
    """EV of a single leg bet of size 'stake'. Return in stake units.
    EV = p * (payoff) + (1-p) * (-stake), payoff = stake*(offered_decimal-1)
    """
    payoff = stake * (offered_decimal - 1.0)
    return true_p * payoff - (1.0 - true_p) * stake


def kelly_fraction(true_p: float, offered_decimal: float) -> float:
    """Full Kelly fraction for a single outcome with decimal odds.
    b = offered_decimal - 1; f* = (b*p - (1-p)) / b.
    Returns 0 if negative (no bet)."""
    b = offered_decimal - 1.0
    numer = b * true_p - (1 - true_p)
    if b <= 0:
        return 0.0
    f = numer / b
    return max(0.0, f)

# --- Parlays ---

def parlay_true_probability(leg_true_ps: List[float]) -> float:
    """Assuming independence: product of leg probabilities.
    Hook here later for correlation adjustments.
    """
    prod = 1.0
    for p in leg_true_ps:
        prod *= p
    return prod


def parlay_ev(leg_true_ps: List[float], offered_leg_decimals: List[float], stake: float = 1.0) -> Tuple[float, float, float]:
    """Return (parlay_decimal_offered, true_p, EV)."""
    offered_parlay_dec = combine_decimal_odds(offered_leg_decimals)
    true_p = parlay_true_probability(leg_true_ps)
    ev = expected_value(true_p, offered_parlay_dec, stake)
    return offered_parlay_dec, true_p, ev